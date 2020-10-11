#include <string.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <drv/assert.h>
#include <drv/usart0.h>
#include <drv/util.h>

/*----------------------------------------------------------------------------*/
#ifdef USART_DBG_CNTRS
void usart_cntrs_str(usart_cntrs_str_t str, const usart_cntrs_t *cntrs)
{
    if(
        NULL == str
        || NULL == cntrs) return;

    str = xprint8(str, cntrs->byte_cntr);
    str = xprint8(str, cntrs->int_cntr);
    *str++ = '\n';
    *str = '\0';
}
#endif /* USART_DBG_CNTRS */
/*----------------------------------------------------------------------------*/
void usart_rxflags_str(usart_rxflags_str_t str, const usart_rxflags_t *flags)
{
    if(
        NULL == str
        || NULL == flags) return;

    uint8_t i = 0;

    str[i++] = flags->parity_error ?  'P' : '-';
    str[i++] = flags->frame_error ? 'F' : '-';
    str[i++] = flags->overrun_error ? 'O' : '-';
    str[i++] = flags->aborted ? 'a' : '-';
    str[i++] = flags->full ? 'f' : '-';
    str[i++] = flags->empty ? 'e' : '-';
    str[i++] = '\n';
    str[i++] = '\0';
}
/*----------------------------------------------------------------------------*/
typedef struct
{
    const uint8_t *begin;
    const uint8_t *end;
    usart_tx_complete_cb_t complete_cb;
    uintptr_t user_data;
} tx_ctrl_t;

static
tx_ctrl_t tx0_;
#ifdef USART_DBG_CNTRS
static
usart_cntrs_t tx0_cntrs_;

usart_cntrs_t *usart0_tx_cntrs(void) {return &tx0_cntrs_;}
#endif /* USART_DBG_CNTRS */

/*----------------------------------------------------------------------------*/
typedef struct
{
    uint8_t *begin;
    const uint8_t *end;
    uint8_t *next;
    usart_rx_pred_cb_t pred_cb;
    usart_rx_complete_cb_t complete_cb;
    usart_rx_recv_cb_t recv_cb;
    uintptr_t user_data;
} rx_ctrl_t;

static
rx_ctrl_t rx0_;
static
usart_rxflags_t rx0_flags_;
#ifdef USART_DBG_CNTRS
static
usart_cntrs_t rx0_cntrs_;

usart_cntrs_t *usart0_rx_cntrs(void) {return &rx0_cntrs_;}
#endif /* USART_DBG_CNTRS */
/*----------------------------------------------------------------------------*/
const char *usart0_send_str_r(const char *begin, const char *const end)
{
    if(NULL == begin) return begin;

//    USART0_TX_ENABLE();

    while(
        NULL == end && *begin != '\0'
        || NULL != end && begin != end)
    {
        /* wait for USART HW to be ready */
        while(!USART0_TX_READY()) {}
        USART0_TX(*begin);
        ++begin;
    }

    while(!USART0_TX_COMPLETE()) {}
//    USART0_TX_DISABLE();
    return begin;
}
/*----------------------------------------------------------------------------*/
const char *usart0_send_str(const char *str)
{
    return usart0_send_str_r(str, NULL);
}
/*----------------------------------------------------------------------------*/
char *usart0_recv_str(char *begin, const char *const end, char delimiter)
{
    if(
        NULL == begin
        || NULL == end) return begin;

    USART0_RX_ENABLE();

    while(begin != end)
    {
        /* wait for USART HW to be ready */
        while(!USART0_RX_READY()) {}
        USART0_RX(*begin);
        ++begin;

        /* special character received */
        if(*(begin - 1) == delimiter) break;
    }

    USART0_RX_DISABLE();

    return begin;
}
/*----------------------------------------------------------------------------*/
void usart0_send(const uint8_t *begin, const uint8_t *const end)
{
    if(
        NULL == begin
        || NULL == end
        || begin == end) return;

    USART0_TX_ENABLE();

    while(begin != end)
    {
        while(!USART0_TX_READY()) {}
        USART0_TX(*begin);
        ++begin;
    }

    while(!USART0_TX_COMPLETE()) {}
    USART0_TX_DISABLE();
}
/*----------------------------------------------------------------------------*/
void usart0_async_send(
        const uint8_t *begin, const uint8_t *end,
        usart_tx_complete_cb_t complete_cb,
        uintptr_t user_data)
{
    ASSERT(NULL != begin);
    ASSERT(NULL != end);
    ASSERT(end > begin);

    if(
        NULL == tx0_.begin
        && NULL == tx0_.end
        && NULL == tx0_.complete_cb
        && 0 == tx0_.user_data)
    {
        tx0_.begin = begin;
        tx0_.end = end;
        tx0_.complete_cb = complete_cb;
        tx0_.user_data = user_data;

        if(!USART0_TX_ENABLED()) USART0_TX_ENABLE();
        /* as soon as UDRE (USART Data Register Empty) interrupt is enabled
         * it will fire, so dont write data to UDR (data race) */
        USART0_TX_READY_INT_ENABLE();
    }
}
/*----------------------------------------------------------------------------*/
void usart0_async_recv(
        uint8_t *begin, const uint8_t *end,
        usart_rx_pred_cb_t pred_cb,
        usart_rx_complete_cb_t complete_cb,
        uintptr_t user_data)
{
    ASSERT(NULL != begin);
    ASSERT(NULL != end);
    ASSERT(end > begin);
    ASSERT(NULL != complete_cb);

    if(
            NULL == rx0_.begin
            && NULL == rx0_.end
            && NULL == rx0_.next
            && NULL == rx0_.pred_cb
            && NULL == rx0_.complete_cb
            && 0 == rx0_.user_data)
    {
        rx0_.begin = begin;
        rx0_.end = end;
        rx0_.next = begin;
        rx0_.pred_cb = pred_cb;
        rx0_.complete_cb = complete_cb;
        rx0_.user_data = user_data;
        if(!USART0_RX_ENABLED()) USART0_RX_ENABLE();
        USART0_RX_INT_ENABLE();
    }
}
/*----------------------------------------------------------------------------*/
static
void rx_complete(void)
{
    if(
        NULL == rx0_.complete_cb
        /* there is no data in buffer */
        || rx0_.begin == rx0_.next) return;

    uint8_t *begin = rx0_.begin;
    const uint8_t *end = rx0_.next;
    uintptr_t user_data = rx0_.user_data;
    usart_rx_complete_cb_t complete_cb = rx0_.complete_cb;

    memset(&rx0_, 0, sizeof(rx_ctrl_t));

    (*complete_cb)(begin, end, &rx0_flags_, user_data);
}
/*----------------------------------------------------------------------------*/
void usart0_async_recv_complete(void)
{
    const uint8_t sreg = SREG;

    cli();
    USART0_RX_INT_DISABLE();
    rx_complete();
    SREG = sreg;
}
/*----------------------------------------------------------------------------*/
void usart0_async_recv_cb(
    usart_rx_recv_cb_t recv_cb,
    uintptr_t user_data)
{
    ASSERT(NULL != recv_cb);

    if(
        NULL == rx0_.begin
        && NULL == rx0_.end
        && NULL == rx0_.next
        && NULL == rx0_.pred_cb
        && NULL == rx0_.complete_cb
        && NULL == rx0_.recv_cb
        && 0 == rx0_.user_data)
    {
        rx0_.recv_cb = recv_cb;
        rx0_.user_data = user_data;
        if(!USART0_RX_ENABLED()) USART0_RX_ENABLE();
        USART0_RX_INT_ENABLE();
    }
}
/*----------------------------------------------------------------------------*/
ISR(USART_UDRE_vect)
{
    ASSERT(NULL != tx0_.begin);
    ASSERT(NULL != tx0_.end);

#ifdef USART_DBG_CNTRS
    ++tx0_cntrs_.int_cntr;
#endif /* USART_DBG_CNTRS */

    while(
        tx0_.begin != tx0_.end
        && USART0_TX_READY())
    {
        USART0_TX(*tx0_.begin);
        ++tx0_.begin;
#ifdef USART_DBG_CNTRS
        ++tx0_cntrs_.byte_cntr;
#endif /* USART_DBG_CNTRS */
    }

    if(tx0_.begin == tx0_.end)
    {
        USART0_TX_READY_INT_DISABLE();
        USART0_TX_COMPLETE_INT_ENABLE();
    }
}

ISR(USART_TX_vect)
{
    ASSERT(NULL != tx0_.begin);
    ASSERT(NULL != tx0_.end);
    ASSERT(NULL != tx0_.complete_cb);
    ASSERT(tx0_.begin == tx0_.end);

    USART0_TX_COMPLETE_INT_DISABLE();

    usart_tx_complete_cb_t complete_cb = tx0_.complete_cb;
    uintptr_t user_data = tx0_.user_data;

    memset(&tx0_, 0, sizeof(tx_ctrl_t));

    if(NULL != complete_cb) (*complete_cb)(user_data);
}
/*----------------------------------------------------------------------------*/
#ifdef USART0_RX_NO_BUFFERING

ISR(USART_RX_vect)
{
#ifdef USART_DBG_CNTRS
    ++rx0_cntrs_.int_cntr;
#endif /* USART_DBG_CNTRS */
    /*------------------------------------------------------------------------*/
    /* non-buffering continuous async mode */
    ASSERT(NULL == rx0_.end);
    ASSERT(NULL == rx0_.next);
    ASSERT(NULL == rx0_.pred_cb);
    ASSERT(NULL == rx0_.complete_cb);
    ASSERT(NULL != rx0_.recv_cb);

#ifdef USART_DBG_CNTRS
        ++rx0_cntrs_.byte_cntr;
#endif /* USART_DBG_CNTRS */

        rx0_flags_.fop_errors = USART0_FOP_ERRORS();
        (*rx0_.recv_cb)(USART0_RD(), rx0_flags_, rx0_.user_data);
}

#else  /* USART0_RX_NO_BUFFERING */

ISR(USART_RX_vect)
{
#ifdef USART_DBG_CNTRS
    ++rx0_cntrs_.int_cntr;
#endif /* USART_DBG_CNTRS */
    /*------------------------------------------------------------------------*/
    /* non-buffering continuous async mode */
    if(NULL != rx0_.recv_cb)
    {
        ASSERT(NULL == rx0_.end);
        ASSERT(NULL == rx0_.next);
        ASSERT(NULL == rx0_.pred_cb);
        ASSERT(NULL == rx0_.complete_cb);
        ASSERT(NULL != rx0_.recv_cb);

#ifdef USART_DBG_CNTRS
        ++rx0_cntrs_.byte_cntr;
#endif /* USART_DBG_CNTRS */

        rx0_flags_.fop_errors = USART0_FOP_ERRORS();
        (*rx0_.recv_cb)(USART0_RD(), rx0_flags_, rx0_.user_data);
        return;
    }
    /*------------------------------------------------------------------------*/

    /* buffer can not be full on entry to ISR - otherwise
     * data will be lost */
    ASSERT(rx0_.next != rx0_.end);

    while(
        rx0_.next != rx0_.end
        && USART0_RX_READY())
    {
        rx0_flags_.fop_errors = USART0_FOP_ERRORS();

        USART0_RX(*rx0_.next);
#ifdef USART_DBG_CNTRS
        ++rx0_cntrs_.byte_cntr;
#endif /* USART_DBG_CNTRS */
        ++rx0_.next;

        if(
            NULL != rx0_.pred_cb
            && (*rx0_.pred_cb)(rx0_.next - 1, rx0_.user_data))
        {
            rx0_flags_.aborted = 1;
            break;
        }
    }

	rx0_flags_.full = rx0_.next == rx0_.end;
	rx0_flags_.empty = !USART0_RX_READY();

    if(
		rx0_flags_.full
		|| rx0_flags_.aborted)
    {
        USART0_RX_INT_DISABLE();
        rx_complete();
    }
}
#endif /* USART0_RX_NO_BUFFERING */
/*----------------------------------------------------------------------------*/
