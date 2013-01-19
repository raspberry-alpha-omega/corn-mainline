#ifndef LEDBORG_H
#define LEDBORG_H

#include "gpio.h"

void ledborg_set_red(enum gpio_level level);
void ledborg_set_green(enum gpio_level level);
void ledborg_set_blue(enum gpio_level level);

void ledborg_set_all(enum gpio_level level);
void ledborg_set(enum gpio_level red, enum gpio_level green, enum gpio_level blue);
void ledborg_init();

#endif
