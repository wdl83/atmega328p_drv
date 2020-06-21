#pragma  once

#include <stdint.h>

#include <avr/wdt.h>

#include <drv/mem.h>

#define WATCHDOG_TIMEOUT_16ms WDTO_15MS
#define WATCHDOG_TIMEOUT_32ms WDTO_30MS
#define WATCHDOG_TIMEOUT_64ms WDTO_60MS
#define WATCHDOG_TIMEOUT_125ms WDTO_120MS
#define WATCHDOG_TIMEOUT_250ms WDTO_250MS
#define WATCHDOG_TIMEOUT_500ms WDTO_500MS
#define WATCHDOG_TIMEOUT_1000ms WDTO_1S
#define WATCHDOG_TIMEOUT_2000ms WDTO_2S
#define WATCHDOG_TIMEOUT_4000ms WDTO_4S
#define WATCHDOG_TIMEOUT_8000ms WDTO_8S

#define watchdog_enable(timeout) wdt_enable(timeout)
#define watchdog_disable() wdt_disable()
#define watchdog_reset() wdt_reset()
