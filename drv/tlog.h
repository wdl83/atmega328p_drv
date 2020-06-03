#pragma once

#ifdef HOST

#define TLOG_TP()
#define TLOG_PRINTF(fmt, ...)

#else

#include <stdint.h>
#include <inttypes.h>

void tlog_init(char *buf);
void tlog_printf(const char *, ...);
void tlog_clear(void);
const char *tlog_begin(void);
const char *tlog_end(void);
void tlog_dump(void);
uint16_t tlog_timestamp(void);

#define TLOG_TP() tlog_printf("%s:%d", __TLOG_FILE__, __LINE__)

#define TLOG_PRINTF(fmt, ...) tlog_printf(fmt, __VA_ARGS__)

#endif
