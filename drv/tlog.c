#ifndef TLOG_DISABLE

#include <stdarg.h>
#include <stddef.h>
// avr-libc
#include <avr/interrupt.h>

#include "tlog.h"
#include "util.h"


tlog_t tlog_;

void tlog_init(char *buf, size_t capacity)
{
    tlog_.begin = buf;
    tlog_.end = tlog_.begin + capacity - 1;
    tlog_.cur = tlog_.begin;
}

void tlog_append(const char *begin, size_t len)
{
    /* cntr is appended as HEX + 1 space char */
    const uint8_t prefix_len = (sizeof(tlog_.cntr) << 1) + 1;
    const size_t capacity = tlog_.end - tlog_.begin;

    if(len + prefix_len > capacity) return;

    const uint8_t sreg = SREG;
    cli();

    const size_t max_len = tlog_.end - tlog_.cur;

    if(len + prefix_len > max_len)
    {
        /* fill remaining tlog buffer with zeros */
        *(tlog_.cur - 1) = '\n';
        memset(tlog_.cur, '\0', max_len);
        /* wrap around and retry */
        tlog_.cur = tlog_.begin;
    }

    tlog_.cur = xprint8(tlog_.cur, tlog_.cntr);
    *tlog_.cur++ = ' ';
    memcpy(tlog_.cur, begin, len);
    tlog_.cur += len;
    ++tlog_.cntr;

    SREG = sreg;
}

void tlog_clear(void)
{
    const uint8_t sreg = SREG;
    cli();

    tlog_.cur = tlog_.begin;

    SREG = sreg;
}

const char *tlog_begin(void)
{
    return tlog_.begin;
}

const char *tlog_end(void)
{
    return tlog_.end;
}

#endif /* TLOG_DISABLE */
