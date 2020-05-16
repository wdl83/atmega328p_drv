#include <stddef.h>

#include <avr/interrupt.h>

#include <drv/tmr0.h>

typedef struct
{
    timer_cb_t cb;
    uintptr_t user_data;
} ctrl_t;

static
ctrl_t timer0_ctrl_;

void timer0_cb(timer_cb_t cb, uintptr_t user_data)
{
    timer0_ctrl_.cb = cb;
    timer0_ctrl_.user_data = user_data;
}

ISR(TIMER0_COMPA_vect)
{
    if(timer0_ctrl_.cb) (*timer0_ctrl_.cb)(timer0_ctrl_.user_data);
}
