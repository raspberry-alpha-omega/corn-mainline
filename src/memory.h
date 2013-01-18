#ifndef raspi_h
#define raspi_h

#include <stdint.h>

extern void PUT32(uint32_t addr, uint32_t value);
extern uint32_t GET32(uint32_t addr);
extern void dummy(unsigned int);

#endif
