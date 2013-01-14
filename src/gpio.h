#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init();

void raspi_okled_set(int state);
void raspi_timer_wait(int usec);
void raspi_mini_uart_send_char(char c);
void raspi_mini_uart_send_string(const char* s);
void raspi_mini_uart_send_hex(uint32_t n);
void raspi_mini_uart_send_newline(void);

#endif
