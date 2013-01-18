#include <stdio.h>
#include "../gpio.h"

void raspi_set_gpio_direction(int pin, enum gpio_direction direction) {
  printf("%s: pin=%d dir=%d\n", __FUNCTION__, pin, direction);
}

void raspi_set_gpio_level(int pin, enum gpio_level level) {
  printf("%s: pin=%d level=%d\n", __FUNCTION__, pin, level);
}

enum gpio_level raspi_get_gpio_level(int pin) {
  printf("%s: pin=%d\n", __FUNCTION__, pin);
  return GPIO_LOW;
}

void raspi_okled_set(enum gpio_level level) {
  printf("%s: level=%d\n", __FUNCTION__, level);
}

void raspi_timer_wait(int usec) {
  printf("%s: usec=%d\n", __FUNCTION__, usec);
}

void raspi_mini_uart_send_char(char c) {
  printf("%s: c=%c\n", __FUNCTION__, c);
}

void raspi_mini_uart_send_string(const char* s) {
  printf("%s: s=[%c]\n", __FUNCTION__, s);
}

void raspi_mini_uart_send_hex(uint32_t n) {
  printf("%s: hex=[%08x]\n", __FUNCTION__, n);
}

void raspi_mini_uart_send_newline(void) {
  printf("%s:\n", __FUNCTION__);
}

void raspi_okled_init() {
  printf("%s:\n", __FUNCTION__);
}
void raspi_mini_uart_init() {
  printf("%s:\n", __FUNCTION__);
}
