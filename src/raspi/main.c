#include "gpio.h"
#include "morse.h"

int main(void) {
  gpio_init();

  for (int i = 0; i < 10; ++i) {
	  raspi_mini_uart_send_string("hello?\r\n");
	  raspi_timer_wait(1000000);
  }

  halt("hello world ");
}
