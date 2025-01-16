#pragma once

#include <stddef.h>
#include <stdint.h>


#define length_of(array)                      (sizeof(array) / sizeof(array[0]))
#define null_field(type, field)                          (((type *)NULL)->field)
#define sizeof_field(type, field)                sizeof(null_field(type, field))

char *xprint8(char *, uint8_t);
char *xprint16(char *, uint16_t);
char *scopy(char *dst, const char *src, size_t);
