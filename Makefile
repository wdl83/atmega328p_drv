$(if $(MAKE_UTILS),,$(error MAKE_UTILS is not defined))

include $(MAKE_UTILS)/Makefile.defs

TARGET = libatmega328p_drv
PUBLIC_HEADERS = include/public/atmega328p_drv

CFLAGS += \
	-DSPI0_ISR_ENABLE \
	-I include \
	-I include/public

CSRCS = \
	src/mem.c \
	src/spi0.c \
	src/spi0_async.c \
	src/tlog.c \
	src/tmr0.c \
	src/tmr1.c \
	src/tmr2.c \
	src/usart0.c \
	src/util.c \
	src/watchdog.c

include $(MAKE_UTILS)/Makefile.a_rules
