

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CFG_TUSB_DEBUG
    #undef CFG_TUSB_DEBUG
#endif
#define CFG_TUSB_DEBUG              0

#define CFG_TUSB_RHPORT0_MODE       OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED
#define CFG_TUSB_OS                 OPT_OS_FREERTOS

#ifndef CFG_TUSB_MEM_SECTION
    #define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
    #define CFG_TUSB_MEM_ALIGN       TU_ATTR_ALIGNED(4)
#endif

#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE      64
#endif

#define CFG_TUD_CDC_RX_BUFSIZE      64
#define CFG_TUD_CDC_TX_BUFSIZE      64
#define CFG_TUD_CDC_EP_BUFSIZE      64

#define CFG_TUD_HID_BUFSIZE         64

#define CFG_TUD_MSC_EP_BUFSIZE      64

#ifdef LED_BOARD_ENABLE
#define CFG_TUD_CDC                 2
#else
#define CFG_TUD_CDC                 1
#endif
    
#define CFG_TUD_MSC                 0
#define CFG_TUD_HID                 1
#define CFG_TUD_MIDI                0
#define CFG_TUD_VENDOR              0

#define CFG_TUSB_MCU                OPT_MCU_ESP32S2

#ifdef __cplusplus
}
#endif
