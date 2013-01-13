#include "gpio.h"
#include "morse.h"

int main(void) {
  gpio_init();

  for (int i = 0; i < 1000; ++i) {
	  raspi_mini_uart_send_string("hello?");
    raspi_mini_uart_send_newline();
	  raspi_timer_wait(1000000);
  }

  halt("hello world ");
}
