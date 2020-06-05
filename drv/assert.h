#pragma once


#define TO_STR_IMPL(x) #x
#define TO_STR(x) TO_STR_IMPL(x)


#if defined ASSERT_DISABLE || defined HOST

#define ASSERT(cond)
#define STATIC_ASSERT(cond)
#define STATIC_ASSERT_STRUCT_OFFSET(type, field, offset)

#else /* ASSERT_DISABLE */

#include <avr/interrupt.h>

void dbg_led_on(void);
void dbg_led_off(void);
void dbg_led_toggle(void);
void sos(const char *message, uint16_t code);


#define ASSERT(cond) \
    do \
    { \
        if(!(cond)) \
        { \
            cli(); \
            for(;;) \
            { \
                sos("\n" __TLOG_FILE__, __LINE__); \
            } \
        } \
    } while(0)

#define WARNING(cond) \
    do \
    { \
        if(!(cond)) \
        { \
            const uint8_t sreg = SREG; \
            cli(); \
            tlog_dump(); \
            tlog_clear(); \
            SREG = sreg; \
        } \
    } while(0)

#define STATIC_ASSERT_MSG(cond, msg) \
    typedef char static_assert_##msg[2 * !!(cond) - 1]

#define STATIC_ASSERT_IMPL1(cond, L) \
    STATIC_ASSERT_MSG(cond, at_line_##L)

#define STATIC_ASSERT_IMPL2(cond, L) \
    STATIC_ASSERT_IMPL1(cond, L)

#define STATIC_ASSERT(cond) \
    STATIC_ASSERT_IMPL2(cond, __LINE__)

#define STATIC_ASSERT_STRUCT_OFFSET(type, field, offset) \
    STATIC_ASSERT((uint8_t *)(&((type *)NULL)->field) == (uint8_t*)offset);

#endif /* ASSERT_DISABLE */
