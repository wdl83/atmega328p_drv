#pragma  once

#include <stdint.h>

#include <avr/wdt.h>

#include <drv/mem.h>

#define WATCHDOG_TIMEOUT_16ms UINT8_C(0) /* WDP[3..0] = 0 */
#define WATCHDOG_TIMEOUT_32ms UINT8_C(M1(WDP0))
#define WATCHDOG_TIMEOUT_64ms UINT8_C(M1(WDP1))
#define WATCHDOG_TIMEOUT_125ms UINT8_C(M2(WDP1, WDP0))
#define WATCHDOG_TIMEOUT_250ms UINT8_C(M1(WDP2))
#define WATCHDOG_TIMEOUT_500ms UINT8_C(M2(WDP2, WDP0))
#define WATCHDOG_TIMEOUT_1000ms UINT8_C(M2(WDP2, WDP1))
#define WATCHDOG_TIMEOUT_2000ms UINT8_C(M3(WDP2, WDP1, WDP0))
#define WATCHDOG_TIMEOUT_4000ms UINT8_C(M1(WDP3))
#define WATCHDOG_TIMEOUT_8000ms UINT8_C(M2(WDP3, WDP0))

void watchdog_enable(uint8_t timeout);
void watchdog_disable(void);
#define watchdog_reset() wdt_reset()
