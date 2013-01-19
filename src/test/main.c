#include <stdio.h>

#include "gpio.h"
#include "pwm.h"

enum gpio_level p17 = GPIO_LOW;
enum gpio_level p21 = GPIO_LOW;
enum gpio_level p22 = GPIO_LOW;

void pin17(enum gpio_level level) {
  raspi_set_gpio_level(17, level);
  p17 = level;
}

void pin21(enum gpio_level level) {
  raspi_set_gpio_level(21, level);
  p21 = level;
}

void pin22(enum gpio_level level) {
  raspi_set_gpio_level(22, level);
  p22 = level;
}

int main() {
  puts("tests go here!");
  pwm_init(0b1111);

  pwm_set_slot(0, &pin17);
  pwm_set_cycle(0, 0b1000);
  pwm_set_slot(1, &pin21);
  pwm_set_cycle(1, 0b0100);
  pwm_set_slot(2, &pin22);
  pwm_set_cycle(2, 0b0010);

  int p17_total = 0;
  int p21_total = 0;
  int p22_total = 0;

  for (int i = 0; i < 16; ++i) {
    pwm_tick();
    if (p17 == GPIO_HIGH) ++ p17_total;
    if (p21 == GPIO_HIGH) ++ p21_total;
    if (p22 == GPIO_HIGH) ++ p22_total;
  }

  printf("p17, cycle=%x total=%d\n", 0b1000, p17_total);
  printf("p21, cycle=%x total=%d\n", 0b0100, p21_total);
  printf("p22, cycle=%x total=%d\n", 0b0010, p22_total);
}
