#pragma once

#include <drv/spi0.h>


void spi0_async(uint8_t *begin, const uint8_t *end, spi_complete_cb_t, uintptr_t);
