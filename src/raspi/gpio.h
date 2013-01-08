#ifndef GPIO_H
#define GPIO_H

void gpio_init();

void raspi_okled_set(int state);
void raspi_timer_wait(int usec);

#endif
