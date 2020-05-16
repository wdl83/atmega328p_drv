#include <stddef.h>

#include <avr/interrupt.h>

#include <drv/tmr1.h>

typedef struct
{
    timer_cb_t cb;
    uintptr_t user_data;
} ctrl_t;

static
ctrl_t timer1_ctrl_;

void timer1_cb(timer_cb_t cb, uintptr_t user_data)
{
    timer1_ctrl_.cb = cb;
    timer1_ctrl_.user_data = user_data;
}

ISR(TIMER1_COMPA_vect)
{
    if(timer1_ctrl_.cb) (*timer1_ctrl_.cb)(timer1_ctrl_.user_data);
}
