#include "gpio.h"
#include "ledborg.h"

#define RED_PIN 17
#define GREEN_PIN 21
#define BLUE_PIN 22

void ledborg_set_all(enum gpio_level level) {
  ledborg_set(level, level, level);
}

void ledborg_set(enum gpio_level red, enum gpio_level green, enum gpio_level blue) {
  raspi_set_gpio_level(17, red);
  raspi_set_gpio_level(21, green);
  raspi_set_gpio_level(22, red);
}

void ledborg_init() {
  raspi_set_gpio_direction(RED_PIN, GPIO_OUTPUT);
  raspi_set_gpio_direction(GREEN_PIN, GPIO_OUTPUT);
  raspi_set_gpio_direction(BLUE_PIN, GPIO_OUTPUT);
}


