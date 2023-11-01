#include <driver/gpio.h>
#include <driver/adc_common.h>

// Button pin
const gpio_num_t PIN_BT_L1 = GPIO_NUM_13;
const gpio_num_t PIN_BT_L2 = GPIO_NUM_14;  // GPIO_NUM_34 for Rev.1
const gpio_num_t PIN_BT_L3 = GPIO_NUM_35;
const gpio_num_t PIN_BT_LS = GPIO_NUM_36;
const gpio_num_t PIN_BT_LM = GPIO_NUM_37;
const gpio_num_t PIN_BT_R1 = GPIO_NUM_38;
const gpio_num_t PIN_BT_R2 = GPIO_NUM_39;
const gpio_num_t PIN_BT_R3 = GPIO_NUM_40;
const gpio_num_t PIN_BT_RS = GPIO_NUM_41;
const gpio_num_t PIN_BT_RM = GPIO_NUM_42;

// Button LED pin
const gpio_num_t PIN_LED_L1 = GPIO_NUM_12;
const gpio_num_t PIN_LED_L2 = GPIO_NUM_11;
const gpio_num_t PIN_LED_L3 = GPIO_NUM_10;
const gpio_num_t PIN_LED_LS = GPIO_NUM_4;
const gpio_num_t PIN_LED_R1 = GPIO_NUM_21;
const gpio_num_t PIN_LED_R2 = GPIO_NUM_33;
const gpio_num_t PIN_LED_R3 = GPIO_NUM_34;
const gpio_num_t PIN_LED_RS = GPIO_NUM_5;

// RGB LED Strip pin
const gpio_num_t PIN_RGB_LED = GPIO_NUM_9;  // GPIO_NUM_10 for Rev.1

// Aime reader I2C pin
const gpio_num_t PIN_I2C_SCL = GPIO_NUM_16;
const gpio_num_t PIN_I2C_SDA = GPIO_NUM_17;

// Lever ADC pin
// see definition of adc1_channel_t, the number below is not pin
const adc1_channel_t PIN_LEVER = ADC1_CHANNEL_5;

// Active level
const uint32_t BT_EN = 1;
const uint32_t BT_SIDE_EN = 0;
const uint32_t LED_EN = 1;
