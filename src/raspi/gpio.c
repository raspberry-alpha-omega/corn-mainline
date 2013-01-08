#include <stdint.h>

#include "raspi.h"
#include "gpio.h"

#define TIMER_  0x20003000
#define COUNTER 0x20003004
#define OKLED (1<<16)

void raspi_okled_init() {
  uint32_t sel = GET32(GPFSEL1);
  sel &= ~(0b111 << 18);
  sel |= (0b001 << 18);
  PUT32(GPFSEL1,sel);
}

void raspi_okled_set(int state) {
  if (state) {
    PUT32(GPCLR0, OKLED);
  } else {
    PUT32(GPSET0, OKLED);
  }
}

void raspi_timer_wait(int usec) {
  uint32_t start = GET32(COUNTER);
  uint32_t end = start + usec;
  while (GET32(COUNTER) < end)
    ;
}

void gpio_init() {
  raspi_okled_init();
}
