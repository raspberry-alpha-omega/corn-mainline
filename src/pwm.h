#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "gpio.h"

#define PWM_SLOTS 8

void pwm_init(uint32_t mask);
void pwm_set_slot(int slot, switch_fn fn);
void pwm_set_cycle(int slot, uint32_t bits);
void pwm_tick();

#endif
