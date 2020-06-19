#include "watchdog.h"

#include <avr/interrupt.h>
#include <avr/io.h>

/* source: ATMEL 7810D-AVR-01/15 */
void watchdog_enable(uint8_t timeout)
{
    cli();
    watchdog_reset();
    WDTCSR |= M2(WDCE, WDE); /* start timed sequence */
    WDTCSR |= M1(WDE) | timeout;
    sei();
}

void watchdog_disable(void)
{
    cli();
    watchdog_reset();
    /* Clear WDRF in MCUSR (it overrides WDE in WDTCSR!) */
    MCUSR &= ~(1 << WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler setting to prevent unintentional time-out */
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    /* Turn off WDT */
    WDTCSR = 0;
    sei();
}
