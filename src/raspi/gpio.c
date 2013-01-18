#include <stdint.h>
#include <stdint.h>

#include "hardware.h"
#include "memory.h"
#include "gpio.h"

static uint32_t* __gpio = (uint32_t*)GPIO_BASE;

// BCM2835-ARM-Peripherals.pdf section 6.1
// Each GPFSELn has 10 x 3 bits (with bits 31:30 reserved) 'b000 = i/p, 'b001 = o/p, others = alternate uses
// Always use INP_GPIO(x) before using OUT_GPIO(x) as this will force 000 before setting 001

#define INP_GPIO(g) *(__gpio+((g)/10)) &= ~(7<<(((g)%10)*3)) // existing bits ANDed with 111..111000111..111
#define OUT_GPIO(g) *(__gpio+((g)/10)) |=  (1<<(((g)%10)*3)) // existing bits ORed  with 000..000001000..000

// GPSETn is at PERI_BASE + GPIO_BASE + 0x1C bytes. 0x1C = 28 bytes = 7 dwords
#define GPIO_SET *(__gpio+7)  // assigning 1<<n will set bit(n) only

// GPCLRn is at PERI_BASE + GPIO_BASE + 0x28 bytes. 0x28 = 40 bytes = 10 dwords
#define GPIO_CLR *(__gpio+10) // assigning 1<<n will clear bit(n) only

#define OKLED (1<<16)

#define PIN_BIT(pin) (1 << pin)

void raspi_set_gpio_direction(int pin, enum gpio_direction direction) {
  INP_GPIO(pin);
  if (GPIO_OUTPUT == direction) {
    OUT_GPIO(pin);
  }
}

void raspi_set_gpio_level(int pin, enum gpio_level level) {
  PUT32(level ? GPCLR0 : GPSET0, PIN_BIT(pin));
}

enum gpio_level raspi_get_gpio_level(int pin) {
  return GPIO_LOW; // TODO how?
}

void raspi_okled_init() {
  raspi_set_gpio_direction(16, GPIO_OUTPUT);
}

void raspi_okled_set(enum gpio_level level) {
  raspi_set_gpio_level(16, level);
}

void raspi_timer_wait(int usec) {
  uint32_t start = GET32(COUNTER);
  uint32_t end = start + usec;
  while (GET32(COUNTER) < end)
    ;
}

void raspi_mini_uart_init() {
    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);

    uint32_t ra = GET32(GPFSEL1);
    ra &= ~(7<<12); //gpio14
    ra |= 2<<12;    //alt5
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    for(ra=0; ra<150; ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,2);
}

void raspi_mini_uart_wait_for_write() {
  while(1) {
    if (GET32(AUX_MU_LSR_REG)&0x20) break;
  }
}

void raspi_mini_uart_write_char(char c) {
    PUT32(AUX_MU_IO_REG, c);
}

void raspi_mini_uart_send_char(char c) {
  raspi_mini_uart_wait_for_write();
  raspi_mini_uart_write_char(c);
}

void raspi_mini_uart_send_string(const char* s) {
  while (*s) {
    raspi_mini_uart_send_char(*s++);
  }
}

const char* hex_chars = "0123456789ABCDEF";

void raspi_mini_uart_send_hex(uint32_t n) {
  char hex[8+1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  for (int i = 7; i >= 0; --i) {
    hex[i] = hex_chars[n & 0xF];
    n >>= 4;
  }
  raspi_mini_uart_send_string(hex);
}

void raspi_mini_uart_send_newline(void) {
  raspi_mini_uart_send_char('\r');
  raspi_mini_uart_send_char('\n');
}
