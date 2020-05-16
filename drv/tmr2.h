#pragma once

#include <avr/io.h>

#include <drv/mem.h>
#include <drv/tmrx.h>

/* ATmega328P */

/* TCCR2A Timer/Counter Control Register A */
/* TCCR2B Timer/Counter Control Register B */
#define TMR2_FORCE_OUT_CMP_MATCH_A() TCCR2B |= M1(FOC2A)
#define TMR2_FORCE_OUT_CMP_MATCH_B() TCCR2B |= M1(FOC2B)
/* timer is disabled when there is no clock source */

/*
 *  | CS22 | CS21 | CS20 |
 *  | 0    | 0    | 0    | No clock source (Timer/Counter stopped).
 *  | 0    | 0    | 1    | clk T2S /(no prescaling)
 *  | 0    | 1    | 0    | clk T2S /8 (from prescaler)
 *  | 0    | 1    | 1    | clk T2S /32 (from prescaler)
 *  | 1    | 0    | 0    | clk T2S /64 (from prescaler)
 *  | 1    | 0    | 1    | clk T2S /128 (from prescaler)
 *  | 1    | 1    | 0    | clk T2S /256 (from prescaler)
 *  | 1    | 1    | 1    | clk T2S /1024 (from prescaler) */

#define TMR2_CLK_DISABLE() TCCR2B &= ~M3(CS22, CS21, CS20)
#define TMR2_CLK_DIV_1() TCCR2B |= M1(CS20)
#define TMR2_CLK_DIV_8() TCCR2B |= M1(CS21)
#define TMR2_CLK_DIV_32() TCCR2B |= M2(CS21, CS20)
#define TMR2_CLK_DIV_64() TCCR2B |= M1(CS22)
#define TMR2_CLK_DIV_128() TCCR2B |= M2(CS22, CS20)
#define TMR2_CLK_DIV_256() TCCR2B |= M2(CS22, CS21)
#define TMR2_CLK_DIV_1024() TCCR2B = M3(CS22, CS21, CS20)

#define TMR2_CLK_SRC_T0_FALLING() TCCR2B = M2(CS22, CS21) | (TCCR2B & ~M1(CS20))
#define TMR2_CLK_SRC_T0_RISING() TCCR2B |= M3(CS22, CS21, CS20)

#define TMR2_MODE_FAST_PWM() TCCR2A |= M2(WGM21, WGM20)
#define TMR2_MODE_FAST_PWM_TOP_OCRA() TCCR2B |= M1(WGM22)
#define TMR2_MODE_FAST_PWM_TOP_0xFF() TCCR2B &= ~M1(WGM22)
#define TMR2_MODE_FAST_PWM_OC2B_NON_INVERTING() TCCR2A |= M1(COM2B1)
#define TMR2_MODE_FAST_PWM_OC2B_INVERTING() TCCR2A |= M2(COM2B1, COM2B0)

/* TCNT2 - Timer/Counter Register */
#define TMR2_RD_CNTR(value) TCNT2
#define TMR2_WR_CNTR(value) TCNT2 = (value)

/* OCR2B - Output Compare Register B */
#define TMR2_RD_A() OCR2A
#define TMR2_RD_B() OCR2B
#define TMR2_WR_A(value) OCR2A = (value)
#define TMR2_WR_B(value) OCR2B = (value)

/* TIMSK2 - Timer/Counter Interrupt Mask Register */
#define TMR2_A_INT_ENABLE() TIMSK2 |= M1(OCIE2A)
#define TMR2_B_INT_ENABLE() TIMSK2 |= M1(OCIE2B)
#define TMR2_A_INT_DISABLE() TIMSK2 &= ~M1(OCIE2A)
#define TMR2_B_INT_DISABLE() TIMSK2 &= ~M1(OCIE2B)

/* TIFR2 - Timer/Counter 0 Interrupt Flag Register */
#define TMR2_OUT_CMP_MATCH_A() (TIFR2 & (1 << OCF2A))
#define TMR2_OUT_CMP_MATCH_B() (TIFR2 & (1 << OCF2B))
#define TMR2_OVERFLOW() (TIFR2 & (1 << TOV0))

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
  * |COM2A1| COM2A0 | Fast PWM
  * ----------------------------------------------------------------------------
  * |0     | 0      | Normal port operation, OC2A disconnected.
  * ----------------------------------------------------------------------------
  * |0     | 1      | WGMx2 = 0: Normal port operation, OC0A disconnected.
  * |      |        | WGMx2 = 1: Toggle OC2A on compare match.
  * ----------------------------------------------------------------------------
  * |1     | 0      | Clear OC2A on compare match, set OC2A at BOTTOM,
  * |      |        | (non-inverting mode).
  * ----------------------------------------------------------------------------
  * |1     | 1      | Set OC2A on compare match, clear OC2A at BOTTOM,
  * |      |        | (inverting mode).
  * ----------------------------------------------------------------------------
  * */

/* set/clear timer callback */
void timer2_cb(timer_cb_t cb, uintptr_t user_data);
