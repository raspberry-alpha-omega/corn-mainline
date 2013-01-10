#include <stdint.h>

#include "raspi.h"
#include "gpio.h"

#define TIMER_  0x20003000
#define COUNTER 0x20003004
#define OKLED (1<<16)

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

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
	while(1) 	{
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

void raspi_mini_uart_send_string(char* s) {
	while (*s) {
		raspi_mini_uart_send_char(*s++);
	}
}

void gpio_init() {
  raspi_okled_init();
  raspi_mini_uart_init();
}
