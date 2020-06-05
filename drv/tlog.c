#include <drv/tlog.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <avr/interrupt.h>

typedef struct
{
    char *begin;
    const char *end;
    char *cur;
    uint8_t cntr;
    uint16_t timestamp;
} tlog_t;

tlog_t tlog_;

void tlog_init(char *buf)
{
    tlog_.begin = buf;
    tlog_.end = tlog_.begin + TLOG_SIZE - 1;
    tlog_.cur = tlog_.begin;
}

void tlog_printf(const char *fmt, ...)
{
    va_list ap;
    int len = 0;
    const uint8_t sreg = SREG;
    cli();
    const uint16_t timestamp = tlog_timestamp();
#if 0
    uint16_t diff =
        timestamp > tlog_.timestamp
        ? timestamp - tlog_.timestamp
        : tlog_.timestamp - timestamp;
#endif

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

    if(TLOG_SIZE < len) goto exit;

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
    tlog_.timestamp = timestamp;
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
