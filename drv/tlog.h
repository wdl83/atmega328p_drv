#pragma once

#ifdef TLOG_DISABLE

#define TLOG_INIT(buf, size)
#define TLOG_TP()
#define TLOG_XPRINT8(str, value)
#define TLOG_XPRINT16(str, value)
#define TLOG_XPRINT2x8(str, value1, value2)

#else

#include <stdint.h>
#include <string.h>

#include "util.h"

typedef struct
{
    char *begin;
    const char *end;
    char *cur;
    uint8_t cntr;
} tlog_t;

extern tlog_t tlog_;

void tlog_init(char *buf, size_t capacity);
void tlog_append(const char *, size_t);
void tlog_printf(const char *, ...);
void tlog_clear(void);
const char *tlog_begin(void);
const char *tlog_end(void);
void tlog_dump(void);

#define TO_STRING_IMPL(x) #x
#define TO_STRING(x) TO_STRING_IMPL(x)

#define TLOG_INIT(buf, size) tlog_init(buf, size)

#define TLOG_TP() \
    do \
    { \
        char buf__[20 /* file */ + 1 + /* : */ + 4 /* line */ + 1 /* \n */] = {0}; \
        char *curr__ = scopy(buf__, __TLOG_FILE__, 20); \
        *curr__++ = ':'; \
        curr__ = xprint16(curr__, (uint16_t)__LINE__); \
        *curr__++ = '\n'; \
        tlog_append(buf__, (size_t)(curr__ - buf__)); \
    } while(0);

#define TLOG_XPRINT8(str, value) \
    do \
    { \
        char buf__[17 /* str */ + 2 /* value */ + 1 /* \n */] = {0}; \
        char *curr__ = scopy(buf__, (str), 17); \
        curr__ = xprint8(curr__, (uint8_t)(value)); \
        *curr__++ = '\n'; \
        tlog_append(buf__, (size_t)(curr__ - buf__)); \
    } while(0);

#define TLOG_XPRINT16(str, value) \
    do \
    { \
        char buf__[15 /* str */ + 4 /* value */ + 1 /* \n */] = {0}; \
        char *curr__ = scopy(buf__, (str), 15); \
        curr__ = xprint16(curr__, (uint16_t)(value)); \
        *curr__++ = '\n'; \
        tlog_append(buf__, (size_t)(curr__ - buf__)); \
    } while(0);

#define TLOG_XPRINT2x8(str, value1, value2) \
    TLOG_XPRINT16((str), (uint8_t)((uint16_t)(value1) << 8) | (uint8_t)(value2))

#endif
