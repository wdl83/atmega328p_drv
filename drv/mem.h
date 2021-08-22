#ifndef mem_h
#define mem_h

#include <stddef.h>
#include <stdint.h>

#define WRITE8(addr, data) ((*((volatile uint8_t *)(addr))) = (data))
#define WRITE16(addr, data) ((*((volatile uint16_t *)(addr))) = (data))

#define READ8(addr) (*((volatile uint8_t *)(addr)))
#define READ16(addr) (*((volatile uint16_t *)(addr)))

#define MASK8(addr, mask) WRITE8(addr, READ8(addr) | (mask))
#define UNMASK8(addr, mask) WRITE8(addr, READ8(addr) & (~(mask)))

#define MASK16(addr, mask) WRITE16(addr, READ16(addr) | (mask))
#define UNMASK16(addr, mask) WRITE16(addr, READ16(addr) & (~(mask)))

#define BITMOD(value, bitNo, bitValue) (value = bitValue ? value | (1 << bitNo) : value & (~(1 << bitNo)))

#define TO_PTR(addr)\
                    ((uint8_t *)(\
                                (uintptr_t)(*(const uint8_t *)(addr)) << 8\
                                | (uintptr_t)(*((const uint8_t *)(addr) + 1))))

#define M1(I1) (UINT8_C(1) << I1)
#define M2(I1, I2) (M1(I1) | UINT8_C(1) << I2)
#define M3(I1, I2, I3) (M2(I1, I2) | UINT8_C(1) << I3)
#define M4(I1, I2, I3, I4) (M3(I1, I2, I3) | UINT8_C(1) << I4)
#define M5(I1, I2, I3, I4, I5) (M4(I1, I2, I3, I4) | UINT8_C(1) << I5)
#define M6(I1, I2, I3, I4, I5, I6) (M5(I1, I2, I3, I4, I5) | UINT8_C(1) << I6)
#define M7(I1, I2, I3, I4, I5, I6, I7) (M6(I1, I2, I3, I4, I5, I6) | UINT8_C(1) << I7)

#endif /* mem_h */
