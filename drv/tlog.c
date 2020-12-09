#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <avr/interrupt.h>

#include <drv/tlog.h>
#include <drv/util.h>


typedef struct
{
    char *begin;
    const char *end;
    char *cur;
    uint8_t cntr;
} tlog_t;

tlog_t tlog_;

void tlog_init(char *buf)
{
    tlog_.begin = buf;
    tlog_.end = tlog_.begin + TLOG_SIZE - 1;
    tlog_.cur = tlog_.begin;
}

void tlog_append(const char *begin, size_t len)
{
    /* cntr is appended as HEX + 1 space char */
    const uint8_t prefix_len = (sizeof(tlog_.cntr) << 1) + 1;

    if(TLOG_SIZE < len + prefix_len) return;

    const uint8_t sreg = SREG;
    cli();

    const size_t capacity = tlog_.end - tlog_.cur;

    if(capacity < len + prefix_len)
    {
        /* fill remaining tlog buffer with zeros */
        *(tlog_.cur - 1) = '\n';
        memset(tlog_.cur, '\0', capacity);
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
#if 0
void tlog_printf(const char *fmt, ...)
{
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
    SREG = sreg;
}
#endif

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
