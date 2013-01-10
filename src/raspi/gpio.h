#ifndef GPIO_H
#define GPIO_H

void gpio_init();

void raspi_okled_set(int state);
void raspi_timer_wait(int usec);
void raspi_mini_uart_send_char(char c);
void raspi_mini_uart_send_string(char* s);

#endif
