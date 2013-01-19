#include "pwm.h"

struct pwm_slot {
  switch_fn fn;
  uint32_t cycle;
  enum gpio_level state;
};

static struct pwm_slot slots[PWM_SLOTS];
static uint32_t pwm_mask;
static uint32_t pwm_clock;

void pwm_init(uint32_t mask) {
  for (int i = 0; i < PWM_SLOTS; ++i) {
    slots[i].fn = 0;
    slots[i].cycle = 0;
    slots[i].state = 0;
  }
  pwm_clock = 0;
  pwm_mask = 0xFF;
}

void pwm_set_slot(int slot, switch_fn fn) {
  slots[slot].fn = fn;
}

void pwm_set_cycle(int slot, uint32_t bits) {
  slots[slot].cycle = bits;
}

void pwm_tick() {
  pwm_clock = (pwm_clock + 1) & pwm_mask;
  for (int i = 0; i < PWM_SLOTS; ++i) {
    struct pwm_slot* slot = &slots[i];
    if (slot->fn) {
      enum gpio_level next = slot->cycle >= pwm_clock ? GPIO_HIGH : GPIO_LOW;
      if (next != slot->state) {
        slot->fn(next);
        slot->state = next;
      }
    }
  }
}
