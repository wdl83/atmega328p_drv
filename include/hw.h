#pragma once

#include <stdint.h>

#define SRAM_SIZE                                                     (1024 * 2)

#define FLASH_SIZE_IN_WORDS                                          (1024 * 16)
#define FLASH_SIZE_IN_BYTES                           (FLASH_SIZE_IN_WORDS << 1)

#define FLASH_PAGE_SIZE_IN_WORDS                                              64
#define FLASH_PAGE_SIZE_IN_BYTES                 (FLASH_PAGE_SIZE_IN_WORDS << 1)

typedef union
{
    struct {
        uint8_t version : 4;
        uint8_t : 4;
    };
    uint8_t value;
} hw_info_t;

#define HW_INFO_UNDEFINED                                             UINT8_C(0)

hw_info_t hw_info();
void hw_init(uint8_t);
