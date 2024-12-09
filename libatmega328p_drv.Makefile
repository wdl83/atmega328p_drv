include Makefile.defs

OBJ_DIR ?= obj.a
DST_DIR ?= .

TARGET = libatmega328p_drv.a
CFLAGS += \
	-I. \
	-DSPI0_ISR_ENABLE

ifdef RELEASE
	CFLAGS += -DASSERT_DISABLE
endif

CSRCS = \
	drv/mem.c \
	drv/spi0.c \
	drv/spi0_async.c \
	drv/tlog.c \
	drv/tmr0.c \
	drv/tmr1.c \
	drv/tmr2.c \
	drv/usart0.c \
	drv/util.c \
	drv/watchdog.c

include Makefile.a.rules
