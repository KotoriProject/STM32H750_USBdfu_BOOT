#include "stc.h"

/*DMX数据区*/
uint8_t dmx_buf[DMX_CHANNEL + 1] __attribute__((section(".ARM.__at_0x24000000")));

/*串口数据缓冲区*/
volatile uint8_t uart4_buf[UART4_MAX_BUF] __attribute__((section(".ARM.__at_0x24000000")));
volatile uint8_t uart7_buf[UART7_MAX_BUF] __attribute__((section(".ARM.__at_0x24000000")));
volatile uint16_t SysTemp_buf[20] __attribute__((section(".ARM.__at_0x24000000")));
volatile uint16_t VBAT_buf[20] __attribute__((section(".ARM.__at_0x24000000")));
