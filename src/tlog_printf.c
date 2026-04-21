#ifndef TLOG_DISABLE

#include <stdio.h>
// avr-libc
#include <avr/interrupt.h>

#include "tlog.h"


void tlog_printf(const char *fmt, ...)
{
    const size_t capacity = tlog_.end - tlog_.begin;
    va_list ap;
    int len = 0;
    const uint8_t sreg = SREG;
    cli();
    va_start(ap, fmt);
retry_index:
    len =
        snprintf(
            tlog_.cur,
            tlog_.end - tlog_.cur,
            //"\n%03" PRIu8 " % 04" PRIX16 " ",
            "\n%03" PRIu8 " ",
            tlog_.cntr/*, diff*/);

    if(tlog_.end - tlog_.cur < len)
    {
        /* wrap around and retry */
        *(tlog_.cur - 1) = '\n';
        tlog_.cur = tlog_.begin;
        goto retry_index;
    }
    else tlog_.cur += len;

retry_data:
    len = vsnprintf(tlog_.cur, tlog_.end - tlog_.cur, fmt, ap);

    if(capacity < len) goto exit;

    if(tlog_.end - tlog_.cur < len)
    {
        /* wrap around and retry */
        *(tlog_.cur - 1) = '\n';
        tlog_.cur = tlog_.begin;
        goto retry_data;
    }
    else tlog_.cur += len;
exit:
    va_end(ap);
    ++tlog_.cntr;
    SREG = sreg;
}

#endif /* TLOG_DISABLE */
