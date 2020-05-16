#pragma once

#include <avr/io.h>

#include <drv/mem.h>
#include <drv/tmrx.h>

/* ATmega328P */

/* TCCR0A/B Timer/Counter Control Register A/B */
#define TMR0_FORCE_OUT_CMP_MATCH_A() TCCR0B |= M1(FOC0A)
#define TMR0_FORCE_OUT_CMP_MATCH_B() TCCR0B |= M1(FOC0B)

/*
 * | CS02 | CS01 | CS00 | Description
 * | 0    | 0    | 0    | No clock source (Timer/Counter stopped)
 * | 0    | 0    | 1    | clk I/O /(no prescaling)
 * | 0    | 1    | 0    | clk I/O /8 (from prescaler)
 * | 0    | 1    | 1    | clk I/O /64 (from prescaler)
 * | 1    | 0    | 0    | clk I/O /256 (from prescaler)
 * | 1    | 0    | 1    | clk I/O /1024 (from prescaler)
 * | 1    | 1    | 0    | External clock source on T0 pin. Clock on falling edge.
 * | 1    | 1    | 1    | External clock source on T0 pin. Clock on rising edge */

#define TMR0_CLK_DISABLE() TCCR0B &= ~M3(CS02, CS01, CS00)
#define TMR0_CLK_RD() (TCCR0B & M3(CS02, CS01, CS00))
#define TMR0_CLK_WR(v) TCCR0B |= (v)
#define TMR0_CLK_DIV_1() TCCR0B |= M1(CS00)
#define TMR0_CLK_DIV_8() TCCR0B |= M1(CS01)
#define TMR0_CLK_DIV_64() TCCR0B |= M2(CS01, S00)
#define TMR0_CLK_DIV_256() TCCR0B |= M1(CS02)
#define TMR0_CLK_DIV_1024() TCCR0B |= M2(CS02, CS00)
#define TMR0_CLK_SRC_T0_FALLING() TCCR0B |= M2(CS02, CS01)
#define TMR0_CLK_SRC_T0_RISING() TCCR0B |= M3(CS02, CS01, CS00)

#define TMR0_MODE_CTC() TCCR0A |= M1(WGM01)

#define TMR0_MODE_PWM_FAST() TCCR0A |= M2(WGM01, WGM00)
#define TMR0_MODE_PWM_FAST_TOP_OCRA() TCCR0B |= M1(WGM02)
#define TMR0_MODE_FAST_PWM_TOP_0xFF() TCCR0B &= ~M1(WGM02)

/* TCNT0 - Timer/Counter Register */
#define TMR0_RD_CNTR() TCNT0
#define TMR0_WR_CNTR(value) TCNT0 = (value)

/* OCR0B - Output Compare Register B */
#define TMR0_RD_A() OCR0A
#define TMR0_RD_B() OCR0B
#define TMR0_WR_A(value) OCR0A = (value)
#define TMR0_WR_B(value) OCR0B = (value)

/* TIMSK0 - Timer/Counter Interrupt Mask Register */
#define TMR0_A_INT_ENABLE() TIMSK0 |= M1(OCIE0A)
#define TMR0_B_INT_ENABLE() TIMSK0 |= M1(OCIE0B)
#define TMR0_A_INT_DISABLE() TIMSK0 &= ~M1(OCIE0A)
#define TMR0_B_INT_DISABLE() TIMSK0 &= ~M1(OCIE0B)
#define TMR0_INT_MASK() TIMSK0

/* TIFR0 - Timer/Counter 0 Interrupt Flag Register */
#define TMR0_OUT_CMP_MATCH_A() (TIFR0 & (1 << OCF0A))
#define TMR0_OUT_CMP_MATCH_B() (TIFR0 & (1 << OCF0B))
#define TMR0_OVERFLOW() (TIFR0 & (1 << TOV0))
#define TMR0_A_INT_CLEAR() TIFR0 |= M1(OCF0A)
#define TMR0_B_INT_CLEAR() TIFR0 |= M1(OCF0B)
#define TMR0_INT_FLAGS() TIFR0

/*
 * |Mode|WGM2|WGM1|WGM0| Operation          | TOP  | OCRx Update | TOV Flag |
 * |0   |0   |0   |0   | Normal             | 0xFF | Immediate   | MAX      |
 * |1   |0   |0   |1   | PWM, phase correct | 0xFF | TOP         | BOTTOM   |
 * |2   |0   |1   |0   | CTC                | OCRA | Immediate   | MAX      |
 * |3   |0   |1   |1   | Fast PWM           | 0xFF | BOTTOM      | MAX      |
 * |4   |1   |0   |0   | Reserved           | ---- | ----------- | -------- |
 * |5   |1   |0   |1   | PWM, phase correct | OCRA | TOP         | BOTTOM   |
 * |6   |1   |1   |0   | Reserved           | ---- | ----------- | -------- |
 * |7   |1   |1   |1   | Fast PWM           | OCRA | BOTTOM      | TOP      |
 * */

/*
 * Compare Output Mode, non-PWM Mode
 *
 * |COM0A1| COM0A0 |
 * ----------------------------------------------------------------------------
 * |0     | 0      | Normal port operation, OC0A disconnected.
 * ----------------------------------------------------------------------------
 * |0     | 1      | Toggle OC0A on compare match
 * ----------------------------------------------------------------------------
 * |1     | 0      | Clear OC0A on compare match
 * ----------------------------------------------------------------------------
 * |1     | 1      | Set OC0A on compare match
 * ----------------------------------------------------------------------------

 * Fast PWM Mode
 *
 * |COM0A1| COM0A0 |
 * ----------------------------------------------------------------------------
 * |0     | 0      | Normal port operation, OC0A disconnected.
 * ----------------------------------------------------------------------------
 * |0     | 1      | WGMx2 = 0: Normal port operation, OC0A disconnected.
 * |      |        | WGMx2 = 1: Toggle OC0A on compare match.
 * ----------------------------------------------------------------------------
 * |1     | 0      | Clear OC0A on compare match, set OC2A at BOTTOM,
 * |      |        | (non-inverting mode).
 * ----------------------------------------------------------------------------
 * |1     | 1      | Set OC0A on compare match, clear OC2A at BOTTOM,
 * |      |        | (inverting mode).
 * ----------------------------------------------------------------------------
 *
 * Phase Correct PWM Mode
 *
 * |COM0A1| COM0A0 |
 * ----------------------------------------------------------------------------
 * |0     | 0      | Normal port operation, OC0A disconnected.
 * ----------------------------------------------------------------------------
 * |0     | 1      | WGMx2 = 0: Normal port operation, OC0A disconnected
 * |      |        | WGMx2 = 1: Toggle OC0A on compare match
 * ----------------------------------------------------------------------------
 * |1     | 0      | Clear OC0A on compare match when up-counting
 * |      |        | Set OC0A on compare match when down-counting
 * ----------------------------------------------------------------------------
 * |1     | 1      | Set OC0A on compare match when up-counting
 * |      |        | Clear OC0A on compare match when down-counting
 * ----------------------------------------------------------------------------
 * */

/* set/clear timer callback */
void timer0_cb(timer_cb_t cb, uintptr_t user_data);
