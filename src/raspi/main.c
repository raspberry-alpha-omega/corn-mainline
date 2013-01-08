#include "gpio.h"
#include "morse.h"

int main(void) {
  gpio_init();
  halt("hello world ");
}
