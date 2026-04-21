#include <avr/io.h>
#include <util/delay_basic.h>

#include <hw.h>

#include <drv/assert.h>
#include <drv/mem.h>
#include <drv/tlog.h>
#include <drv/tmr1.h>
#include <drv/usart0.h>

#include <stdio.h>

static hw_info_t hw_info_;

hw_info_t hw_info()
{
    return hw_info_;
}

void hw_init(uint8_t raw)
{
    hw_info_.value = raw;
}

#ifndef ASSERT_DISABLE

/* PB5: RED LED */
void dbg_led_on(void)
{
    if(0 == (DDRB & M1(DDB5))) DDRB |= M1(DDB5);
    PORTB |= M1(DDB5);
}

void dbg_led_off(void)
{

    if(0 == (DDRB & M1(DDB5))) DDRB |= M1(DDB5);
    PORTB &= ~M1(DDB5);
}

void dbg_led_toggle(void)
{
    if(0 == (DDRB & M1(DDB5))) DDRB |= M1(DDB5);
    PORTB ^= M1(DDB5);
}

static
void wait(uint8_t cntr)
{
    while(0 != cntr)
    {
        _delay_loop_2(UINT16_MAX);
        --cntr;
    }
}

static
void sos_impl(uint8_t no, uint8_t delay)
{
    while(0 != no)
    {
        dbg_led_on();
        wait(delay);
        dbg_led_off();
        wait(delay);
        --no;
    }
}

void sos(const char *msg, uint16_t code)
{
    char buf[1 /* ' ' */ + 5 /* 2^16 decimal */ + 1 /* \n */ + 1 /* \0 */];
    sprintf(buf, " %05" PRIu16 "\n", code);

    while(0 != code)
    {
        sos_impl((code % 10) + 1, 20);
        wait(50);
        code /= 10;
    }

    usart0_send_str(msg);
    usart0_send_str(buf);
    tlog_dump();
    wait(255);
}

void tlog_dump(void)
{
    char buf[16] = {0};
    usart0_send_str("\n<<<\n");
    snprintf(buf, sizeof(buf), "TLOG %0" PRIu16 "\n", tlog_end() - tlog_begin());
    usart0_send_str(buf);
    usart0_send_str_r(tlog_begin(), tlog_end());
    usart0_send_str("\n>>>\n");
}

uint16_t tlog_timestamp(void)
{
    return TMR1_RD16_CNTR();
}

#endif /* ASSERT_DISABLE */
