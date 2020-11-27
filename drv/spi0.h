#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

#include <drv/mem.h>

/* SPCR - SPI Control Register */
#define SPI0_INT_ENABLE() SPCR |= M1(SPIE)
#define SPI0_INT_DISABLE() SPCR &= ~M1(SPIE)
#define SPI0_ENABLE() SPCR |= M1(SPE)
#define SPI0_DISABLE() SPCR &= ~M1(SPE)
#define SPI0_ORDER_LSB() SPCR |= M1(DORD)
#define SPI0_ORDER_MSB() SPCR &= ~M1(DORD)
#define SPI0_MASTER() SPCR |= M1(MSTR)
#define SPI0_SLAVE() SPCR &= ~M1(MSTR)
#define SPI0_SCK_IDLE_HI() SPCR |= M1(CPOL)
#define SPI0_SCK_IDLE_LOW() SPCR &= ~M1(CPOL)
/* data sampling point relative to SCK clock */
#define SPI0_SCK_LEADING_SAMPLE_TRAILING_SETUP() SPCR &= ~M1(CPHA)
#define SPI0_SCK_LEADING_SETUP_TRAILING_SAMPLE() SPCR |= M1(CPHA)

#define SPI0_CLK_DIV_4() SPCR &= M2(SPR1, SPR0)
#define SPI0_CLK_DIV_16() SPCR = (SPCR & ~M1(SPR1)) | M1(SPR0)
#define SPI0_CLK_DIV_64() SPCR = M1(SPR1) | (SPCR & ~M1(SPR0))
#define SPI0_CLK_DIV_128() SPCR |= M2(SPR1, SPR0)
#define SPI0_CLK_SCALE() (SPCR & M2(SPR1, SPR0))

/* SPSR - SPI Status Register */
#define SPI0_CLK_x2() SPSR |= M1(SPI2X)
#define SPI0_INT_STATUS() (SPSR & M1(SPIF))
#define SPI0_COLISION_STATUS() (SPSR & M1(WCOL))

/* SPDR - SPI Data Register */
#define SPI0_WR(value) SPDR = (value)
#define SPI0_RD() SPDR

#define SPI0_COMPLETE() SPI0_INT_STATUS()

void spi0_xchg(uint8_t *begin, const uint8_t *const end);

#ifdef SPI0_ISR_ENABLE
typedef void (*spi_complete_cb_t)(uintptr_t);
void spi0_complete_cb(spi_complete_cb_t, uintptr_t);
#endif
