#include <stddef.h>
#include <string.h>

#include <avr/interrupt.h>

#include <drv/assert.h>
#include <drv/spi0.h>

#include <drv/usart0.h>

typedef struct
{
    spi_complete_cb_t complete_cb;
    uintptr_t user_data;
} ctrl_t;

static
ctrl_t ctrl0_;

void spi0_complete_cb(spi_complete_cb_t complete_cb, uintptr_t user_data)
{
    ctrl0_.complete_cb = complete_cb;
    ctrl0_.user_data = user_data;
}

/* SPI Serial Transfer Complete */
ISR(SPI_STC_vect)
{
    ASSERT(ctrl0_.complete_cb);
    (*ctrl0_.complete_cb)(ctrl0_.user_data);
}
