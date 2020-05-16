#include <stddef.h>

#include <avr/interrupt.h>

#include <drv/tmr2.h>

typedef struct
{
    timer_cb_t cb;
    uintptr_t user_data;
} ctrl_t;

static
ctrl_t timer2_ctrl_;

void timer2_cb(timer_cb_t cb, uintptr_t user_data)
{
    timer2_ctrl_.cb = cb;
    timer2_ctrl_.user_data = user_data;
}

ISR(TIMER2_COMPA_vect)
{
    if(timer2_ctrl_.cb) (*timer2_ctrl_.cb)(timer2_ctrl_.user_data);
}
