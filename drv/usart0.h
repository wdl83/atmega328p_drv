#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

#include <drv/mem.h>

/*----------------------------------------------------------------------------*/
#define USART0_TX_ENABLE() UCSR0B |= M1(TXEN0)
#define USART0_TX_DISABLE() UCSR0B &= ~M1(TXEN0)
#define USART0_TX_ENABLED() (UCSR0B & M1(TXEN0))

#define USART0_RX_ENABLE() UCSR0B |= M1( RXEN0)
#define USART0_RX_DISABLE() UCSR0B &= ~M1( RXEN0)
#define USART0_RX_ENABLED() (UCSR0B & M1(RXEN0))

#define CALC_BR(cpu_clk, bps) (((cpu_clk) >> 4) / (bps) - 1)
#define USART0_BR(br) (UBRR0H = (uint8_t)((br) >> 8), UBRR0L = (uint8_t)(br))

#define USART0_TX_READY() (UCSR0A & M1(UDRE0))
#define USART0_TX_COMPLETE() (UCSR0A & M1(TXC0))

#define USART0_RX_READY() (UCSR0A & M1(RXC0))

#define USART0_TX(data) UDR0 = (data)
#define USART0_RX(data) (data) = UDR0
#define USART0_RD() UDR0

#define USART0_TX_READY_INT_ENABLE() UCSR0B |= M1(UDRIE0)
#define USART0_TX_READY_INT_DISABLE() UCSR0B &= ~M1(UDRIE0)

#define USART0_TX_COMPLETE_INT_ENABLE() UCSR0B |= M1(TXCIE0)
#define USART0_TX_COMPLETE_INT_DISABLE() UCSR0B &= ~M1(TXCIE0)

#define USART0_RX_INT_ENABLE() UCSR0B |= M1(RXCIE0)
#define USART0_RX_INT_DISABLE() UCSR0B &= ~M1(RXCIE0)

#define USART0_PARITY_EVEN() UCSR0C |= M1(UPM01)
#define USART0_PARITY_ODD() UCSR0C |= M2(UPM01, UPM00)

/*
 * USART Control and Status Register A
 * 7    6    5     4   3    2    1    0
 * RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
 * R    R/W  R     R   R    R    R/W  R/W
 * 0    0    1     0   0    0    0    0
*/

#define USART0_FRAME_ERROR() (UCSR0A & M1(FE0))
#define USART0_OVERRUN_ERROR() (UCSR0A & M1(DOR0))
#define USART0_PARITY_ERROR() (UCSR0A & M1(UPE0))

#define USART0_FOP_ERRORS() ((UCSR0A & M3(FE0, DOR0, UPE0)) >> 2)

/*----------------------------------------------------------------------------*/
#ifdef USART_DBG_CNTRS
typedef union
{
    struct
    {
        uint8_t byte_cntr : 8;
        uint8_t int_cntr : 8;
    };
    uint16_t value;
} usart_cntrs_t;

typedef
    char usart_cntrs_str_t[
        2 /* ?? : byte counter */ +
        2 /* ?? : interrupt counter */ +
        2 /* \n\0 */];

usart_cntrs_t *usart0_tx_cntrs(void);
usart_cntrs_t *usart0_rx_cntrs(void);
void usart_cntrs_str(usart_cntrs_str_t, const usart_cntrs_t *);
#endif /* USART_DBG_CNTRS */
/*----------------------------------------------------------------------------*/

typedef union
{
    struct
    {
        uint8_t aborted : 1;
        uint8_t full : 1;
        uint8_t empty : 1;
        uint8_t : 1;
        uint8_t : 1;
        uint8_t frame_error : 1;
        uint8_t overrun_error : 1;
        uint8_t parity_error : 1;
    };

    struct
    {
        uint8_t : 4;
        uint8_t : 1;
        uint8_t fop_errors : 3;
    };

    uint8_t value;
} usart_rxflags_t;

typedef
    char usart_rxflags_str_t[
        1 /* P  : parity error */ +
        1 /* F  : frame error */ +
        1 /* O  : overrun error */ +
        1 /* a  : aborted */ +
        1 /* f  : full */ +
        1 /* e  : empty */ +
        2 /* \n\0 */];

void usart_rxflags_str(usart_rxflags_str_t, const usart_rxflags_t *);

typedef
    char usart_txflags_str_t[
        1 /* \n */ +
        2 /* ?? : byte counter */ +
        2 /* ?? : interrupt counter */ +
        2 /* \n\0 */];

typedef
void (*usart_tx_complete_cb_t)(uintptr_t);

typedef
bool (*usart_rx_pred_cb_t)(const uint8_t *curr, uintptr_t);

typedef
void (*usart_rx_complete_cb_t)(
    uint8_t *begin, const uint8_t *end,
    usart_rxflags_t *,
    uintptr_t user_data);

typedef
void (*usart_rx_recv_cb_t)(
    uint8_t data,
    usart_rxflags_t,
    uintptr_t user_data);

const char *usart0_send_str_r(const char *begin, const char *const end);
const char *usart0_send_str(const char *str);
char *usart0_recv_str(char *begin, const char *const end, char delimiter);

void usart0_send(const uint8_t *begin, const uint8_t *const end);

void usart0_async_send(
    const uint8_t *begin, const uint8_t *end,
    usart_tx_complete_cb_t,
    uintptr_t);

void usart0_async_recv(
    uint8_t *begin, const uint8_t *end,
    usart_rx_pred_cb_t,
    usart_rx_complete_cb_t,
    uintptr_t);

void usart0_async_recv_complete(void);

/* non-buffering continuous async mode */
void usart0_async_recv_cb(
    usart_rx_recv_cb_t recv_cb,
    uintptr_t user_data);
/*----------------------------------------------------------------------------*/
