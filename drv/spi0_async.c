#include <stddef.h>
#include <string.h>

#include <avr/interrupt.h>

#include <drv/assert.h>
#include <drv/spi0.h>

#include <drv/usart0.h>

typedef struct
{
    uint8_t *begin;
    const uint8_t *end;
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

void spi0_async(
    uint8_t *begin, const uint8_t *end,
    spi_complete_cb_t complete_cb,
    uintptr_t user_data)
{
    ASSERT(begin);
    ASSERT(end);
    ASSERT(complete_cb);

    if(
        ctrl0_.begin
        || ctrl0_.end
        || ctrl0_.complete_cb
        || ctrl0_.user_data) return;

    ctrl0_.begin = begin;
    ctrl0_.end = end;
    ctrl0_.complete_cb = complete_cb;
    ctrl0_.user_data = user_data;
    SPI0_ENABLE();
    SPI0_WR(*ctrl0_.begin);
    SPI0_INT_ENABLE();
}

/* SPI Serial Transfer Complete */
ISR(SPI_STC_vect)
{
    /* TODO: verify */
    if(ctrl0_.begin != ctrl0_.end)
    {
        *ctrl0_.begin = SPI0_RD();
        ++ctrl0_.begin;
        SPI0_WR(*ctrl0_.begin);

        if(ctrl0_.begin == ctrl0_.end)
        {
            spi_flags_t flags = ctrl0_.flags;
            spi_complete_cb_t complete_cb = ctrl0_.complete_cb;
            const uintptr_t user_data = ctrl0_.user_data;

            memset(&ctrl0_, 0, sizeof(ctrl_t));

            if(NULL != complete_cb) (*complete_cb)(&flags, user_data);
        }
    }
}
