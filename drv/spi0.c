#include <stddef.h>
#include <string.h>

#include <avr/interrupt.h>

#include <drv/assert.h>
#include <drv/spi0.h>

#include <drv/usart0.h>


void spi0_xchg(uint8_t *begin, const uint8_t *const end)
{
    while(begin != end)
    {
        SPI0_WR(*begin);

        while(!SPI0_COMPLETE()) {}

        *begin = SPI0_RD();
        ++begin;
    }
}

#ifdef SPI0_ISR_ENABLE

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
    (*ctrl0_.complete_cb)(ctrl0_.user_data);
}

#endif /* SPI0_ISR_ENABLE */
