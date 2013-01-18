#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

enum gpio_direction { GPIO_INPUT, GPIO_OUTPUT };
void raspi_set_gpio_direction(int pin, enum gpio_direction direction);

enum gpio_level { GPIO_LOW=0, GPIO_HIGH=1 };
void raspi_set_gpio_level(int pin, enum gpio_level level);

enum gpio_level raspi_get_gpio_level(int pin);

void raspi_okled_set(enum gpio_level level);
void raspi_timer_wait(int usec);
void raspi_mini_uart_send_char(char c);
void raspi_mini_uart_send_string(const char* s);
void raspi_mini_uart_send_hex(uint32_t n);
void raspi_mini_uart_send_newline(void);

void raspi_okled_init();
void raspi_mini_uart_init();

#endif
