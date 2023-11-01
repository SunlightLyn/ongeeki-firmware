#include "stdinclude.hpp"
#include "pin_config.hpp"

#include <driver/gpio.h>
#include <driver/adc_common.h>
#include <led_strip.h>
#include <driver/rmt.h>
#include <lwip/def.h>

namespace hardware {
    // Button and LED array:
    // LA LB LC LS LM
    // RA RB RC RS RM
    
    const gpio_num_t gpio_settings[] = {
        PIN_BT_L1, PIN_BT_L2, PIN_BT_L3, PIN_BT_LS, PIN_BT_LM,
        PIN_BT_R1, PIN_BT_R2, PIN_BT_R3, PIN_BT_RS, PIN_BT_RM,
    };

    const gpio_num_t led_settings[] = {
        PIN_LED_L1, PIN_LED_L2, PIN_LED_L3, PIN_LED_LS, GPIO_NUM_NC,
        PIN_LED_R1, PIN_LED_R2, PIN_LED_R3, PIN_LED_RS, GPIO_NUM_NC,
    };

    const uint32_t gpio_en_level[] = {
        BT_EN, BT_EN, BT_EN, BT_SIDE_EN, BT_EN,
        BT_EN, BT_EN, BT_EN, BT_SIDE_EN, BT_EN,
    };

    const switch_map_t switch_maps[] = {
        // Left A, B, C
        switch_map_t { .player = 0, .bit = 0  },
        switch_map_t { .player = 0, .bit = 5  },
        switch_map_t { .player = 0, .bit = 4  },
        // Left Side, Menu
        switch_map_t { .player = 1, .bit = 15 },
        switch_map_t { .player = 1, .bit = 14 },
        // Right A, B, C
        switch_map_t { .player = 0, .bit = 1  },
        switch_map_t { .player = 1, .bit = 0  },
        switch_map_t { .player = 0, .bit = 15 },
        // Right Side, Menu
        switch_map_t { .player = 0, .bit = 14 },
        switch_map_t { .player = 0, .bit = 13 },
    };

    const adc1_channel_t LEVER_PIN = PIN_LEVER;

    led_strip_t *led;

    void init() {
        for(int i = 0; i < 10; i++) {
            gpio_set_direction(gpio_settings[i], GPIO_MODE_INPUT);
            gpio_set_pull_mode(gpio_settings[i], GPIO_PULLUP_ONLY);
        }

        for(int i = 0; i < 10; i++) {
            gpio_set_direction(led_settings[i], GPIO_MODE_OUTPUT);
            gpio_set_level(led_settings[i], !LED_EN);
        }

        adc1_config_width(ADC_WIDTH_BIT_13);
        adc1_config_channel_atten(LEVER_PIN, ADC_ATTEN_DB_11);

        rmt_config_t config = RMT_DEFAULT_CONFIG_TX(PIN_RGB_LED, RMT_CHANNEL_0);
        config.clk_div = 2;
        rmt_config(&config);
        rmt_driver_install(config.channel, 0, 0);

        led_strip_config_t led_config = LED_STRIP_DEFAULT_CONFIG(6, (led_strip_dev_t) config.channel);
        led = led_strip_new_rmt_ws2812(&led_config);

        led->clear(led, 50);
    }

    void update(usb::io4::output_t &data) {
        int test_sum = 0;

        for(int i = 0; i < 10; i++) {
            auto& map = switch_maps[i];
            auto state = !gpio_get_level(gpio_settings[i]);

            if(state) {
                if(i == 0 || i == 7 || i == 4 || i == 9) {
                    test_sum ++;
                }

                data.switches[map.player] |= 1 << map.bit;
            } else {
                data.switches[map.player] &= ~(1 << map.bit);
            }

            gpio_set_level(led_settings[i], (state == gpio_en_level[i]) == LED_EN);
        }

        static bool is_inserted_coin = false;

        // TEST Workaround: Press both LA, LM, RC and RM to trigger
        if(test_sum == 4) {
            data.switches[0] |= 1 << 9;
            data.switches[0] |= 1 << 6;
        } else if(test_sum == 2) {
            if(!is_inserted_coin) {
                is_inserted_coin = true;

                data.coin[0].count ++;
            }
        }  else {
            data.switches[0] &= ~(1 << 9);
            data.switches[0] &= ~(1 << 6);

            is_inserted_coin = false;
        }

        // Low-pass filter from
        // https://kiritchatterjee.wordpress.com/2014/11/10/a-simple-digital-low-pass-filter-in-c/
        // since ESP32S2 has FPU then we just use float here
        static float smooth_lever = 0;
        const float lpf_beta = 0.25f;

        // auto raw_lever = float(adc1_get_raw(LEVER_PIN) - 0x13FF) * 0x10;
        auto raw_lever = float(adc1_get_raw(LEVER_PIN)) * 0x10;
        smooth_lever = smooth_lever - (lpf_beta * (smooth_lever - raw_lever));

        data.analog[0] = smooth_lever;
    }

    const uint8_t mu3_led_mapping[18] = {1, 0, 3, 5, 4, 2, 8, 6, 7, 11, 9, 10, 14, 12, 13, 17, 15, 16};

    void led_update(const uint8_t *payload) {
        auto leds = ntohl(*(uint32_t *)payload);

        uint8_t out_leds[18];
        for (uint8_t i = 0; i < 18; i++) {
            out_leds[i] = ((leds >> (31 - mu3_led_mapping[i])) & 1) ? 0xff : 0x00;
        }

        for (uint8_t i = 0; i < 6; i++) {
            led->set_pixel(led, i, out_leds[i * 3 + 2], out_leds[i * 3 + 1], out_leds[i * 3 + 0]);
        }

        led->refresh(led, 100);
    }
}

