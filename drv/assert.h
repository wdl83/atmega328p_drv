#pragma once

#include <stddef.h>

#define TO_STR_IMPL(x) #x
#define TO_STR(x) TO_STR_IMPL(x)

#if defined HOST

#define STATIC_ASSERT(cond)
#define STATIC_ASSERT_STRUCT_OFFSET(type, field, offset)

#else

#define STATIC_ASSERT_MSG(cond, msg) \
    typedef struct {uint8_t value : (cond) ? 8 : 0;} static_assert_##msg
    //typedef char static_assert_##msg[2 * !!(cond) - 1]

#define STATIC_ASSERT_IMPL1(cond, L, C) \
    STATIC_ASSERT_MSG(cond, C##_at_line_##L)

#define STATIC_ASSERT_IMPL2(cond, L, C) \
    STATIC_ASSERT_IMPL1(cond, L, C)

#define STATIC_ASSERT(cond) \
    STATIC_ASSERT_IMPL2(cond, __LINE__, __COUNTER__)

#define STRUCT_OFFSET(type, field) \
    (size_t)((const uint8_t *const)(&((const type *const)NULL)->field))

#define STATIC_ASSERT_STRUCT_OFFSET(type, field, offset) \
    STATIC_ASSERT(STRUCT_OFFSET(type, field) == (size_t)(offset))
    //STATIC_ASSERT((const uint8_t *const)(&((const type *const)NULL)->field) == (const uint8_t *const)(offset));

#endif

#if defined ASSERT_DISABLE || defined HOST

#define ASSERT(cond)

#else /* ASSERT_DISABLE */

#include <avr/interrupt.h>

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

#endif /* ASSERT_DISABLE */
