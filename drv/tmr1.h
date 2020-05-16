#pragma once

#include <avr/io.h>

#include <drv/mem.h>
#include <drv/tmrx.h>

/* ATmega328P */

/* TCCR1A/B/C Timer/Counter Control Register A/B/C */
#define TMR1_FORCE_OUT_CMP_MATCH_A() TCCR1C |= M1(FOC1A)
#define TMR1_FORCE_OUT_CMP_MATCH_B() TCCR1C |= M1(FOC1B)

/*
 * | CS12 | CS11 | CS10 | Description
 * | 0    | 0    | 0    | No clock source (Timer/Counter stopped)
 * | 0    | 0    | 1    | clk I/O /(no prescaling)
 * | 0    | 1    | 0    | clk I/O /8 (from prescaler)
 * | 0    | 1    | 1    | clk I/O /64 (from prescaler)
 * | 1    | 0    | 0    | clk I/O /256 (from prescaler)
 * | 1    | 0    | 1    | clk I/O /1024 (from prescaler)
 * | 1    | 1    | 0    | External clock source on T1 pin. Clock on falling edge.
 * | 1    | 1    | 1    | External clock source on T1 pin. Clock on rising edge */

#define TMR1_CLK_DISABLE() TCCR1B &= ~M3(CS12, CS11, CS10)
#define TMR1_CLK_RD() (TCCR1B & M3(CS12, CS11, CS10))
#define TMR1_CLK_WR(v) TCCR1B |= (v)
#define TMR1_CLK_DIV_1() TCCR1B |= M1(CS10)
#define TMR1_CLK_DIV_8() TCCR1B |= M1(CS11)
#define TMR1_CLK_DIV_64() TCCR1B |= M2(CS11, CS10)
#define TMR1_CLK_DIV_256() TCCR1B |= M1(CS12)
#define TMR1_CLK_DIV_1024() TCCR1B |= M2(CS12, CS10)
#define TMR1_CLK_SRC_T0_FALLING() TCCR1B |= M2(CS12, CS11)
#define TMR1_CLK_SRC_T0_RISING() TCCR1B |= M3(CS12, CS11, CS10)
//
#define TMR1_MODE_CTC() TCCR1B |= M1(WGM12)
//
//#define TMR0_MODE_PWM_FAST() TCCR1A |= M2(WGM01, WGM00)
//#define TMR0_MODE_PWM_FAST_TOP_OCRA() TCCR1B |= M1(WGM02)
//#define TMR0_MODE_FAST_PWM_TOP_0xFF() TCCR1B &= ~M1(WGM02)
//
/* TCNT1 - Timer/Counter Register */
#define TMR1_RD16_CNTR() TCNT1
#define TMR1_WR16_CNTR(value) TCNT1H = (value)
//
///* OCR1B - Output Compare Register B */
#define TMR1_RD16_A() OCR1A
#define TMR1_RD16_B() OCR1B
#define TMR1_WR16_A(value) OCR1A = (value)
#define TMR1_WR16_B(value) OCR1B = (value)
//
///* TIMSK1 - Timer/Counter Interrupt Mask Register */
#define TMR1_IC_INT_ENABLE() TIMSK1 |= M1(ICIE1)
#define TMR1_A_INT_ENABLE() TIMSK1 |= M1(OCIE1A)
#define TMR1_B_INT_ENABLE() TIMSK1 |= M1(OCIE1B)
#define TMR1_A_INT_DISABLE() TIMSK1 &= ~M1(OCIE1A)
#define TMR1_B_INT_DISABLE() TIMSK1 &= ~M1(OCIE1B)
#define TMR1_OVERFLOW_INT_ENABLE() TIMSK1 |= M1(TOIE1)
//
///* TIFR1 - Timer/Counter 0 Interrupt Flag Register */
#define TMR1_IC_FLAG() (TIFR1 & (1 << TOV1))
#define TMR1_OC_MATCH_A_FLAG() (TIFR1 & (1 << OCF1A))
#define TMR1_OC_MATCH_B_FLAG() (TIFR1 & (1 << OCF1B))
#define TMR1_OVERFLOW_FLAG() (TIFR1 & (1 << TOV1))
#define TMR1_A_INT_CLEAR() TIFR1 |= M1(OCF1A)
#define TMR1_B_INT_CLEAR() TIFR1 |= M1(OCF1B)

/*
 * |Mode|WGM13|WGM2|WGM1|WGM0| Operation              | TOP    | OCRx Update | TOV Flag |
 * | 0  | 0   | 0  | 0  | 0  | Normal                 | 0xFFFF | Immediate   | MAX      |
 * | 1  | 0   | 0  | 0  | 1  | PWM phase correct 8bit | 0x00FF | TOP         | BOTTOM   |
 * | 2  | 0   | 0  | 1  | 0  | PWM phase correct 9bit | 0x01FF | TOP         | BOTTOM   |
 * | 3  | 0   | 0  | 1  | 1  | PWM phase correct 10bit| 0x03FF | TOP         | BOTTOM   |
 * | 4  | 0   | 1  | 0  | 0  | CTC                    | OCR1A  | Immediate   | MAX      |
 * | 5  | 0   | 1  | 0  | 1  | Fast PWM 8-bit         | 0x00FF | BOTTOM      | TOP      |
 * | 6  | 0   | 1  | 1  | 0  | Fast PWM 9-bit         | 0x01FF | BOTTOM      | TOP      |
 * | 7  | 0   | 1  | 1  | 1  | Fast PWM 10-bit        | 0x03FF | BOTTOM      | TOP      |
 * | 8  | 1   | 0  | 0  | 0  | PWM phase&freq correct | ICR1   | BOTTOM      | BOTTOM   |
 * | 9  | 1   | 0  | 0  | 1  | PWM phase&freq correct | OCR1A  | BOTTOM      | BOTTOM   |
 * | 10 | 1   | 0  | 1  | 0  | PWM phase correct      | ICR1   | TOP         | BOTTOM   |
 * | 11 | 1   | 0  | 1  | 1  | PWM phase correct      | OCR1A  | TOP         | BOTTOM   |
 * | 12 | 1   | 1  | 0  | 0  | CTC                    | ICR1   | Immediate   | MAX      |
 * | 13 | 1   | 1  | 0  | 1  | (Reserved)             |--------|-------------|----------|
 * | 14 | 1   | 1  | 1  | 0  | Fast PWM               | ICR1   | BOTTOM      | TOP      |
 * | 15 | 1   | 1  | 1  | 1  | Fast PWM               | OCR1A  | BOTTOM      | TOP      |
 */

/*
 * Compare Output Mode, non-PWM Mode
 *
 * |COM1A1| COM1A0 |
 * |COM1B1| COM1B0 |
 * ----------------------------------------------------------------------------
 * |0     | 0      | Normal port operation, OC1A/OC1B disconnected.
 * ----------------------------------------------------------------------------
 * |0     | 1      | Toggle OC1A/OC1B on compare match
 * ----------------------------------------------------------------------------
 * |1     | 0      | Clear OC1A/OC1B on compare match
 * ----------------------------------------------------------------------------
 * |1     | 1      | Set OC1A/OC1B on compare match
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
void timer1_cb(timer_cb_t cb, uintptr_t user_data);
