#ifndef MORSE_H
#define MORSE_H

#include "gpio.h"

typedef void (*switch_fn)(enum gpio_level);
void morse_set_switch(switch_fn fn);

void switch_on(int usec);
void switch_off(int usec);

void morse_glyph(char glyph);
void morse_symbol(const char* symbol);
void morse_char(char c);
void morse_string(const char* message);

void flash_dots(int ndots);
void halt(const char* message);

#endif
