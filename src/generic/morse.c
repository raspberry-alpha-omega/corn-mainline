#include "morse.h"
#include "gpio.h"

struct Morse { char letter; const char* symbol; } code[] = {
    { 'a', ".-" },
    { 'b', "-..." },
    { 'c', "-.-." },
    { 'd', "-.." },
    { 'e', "." },
    { 'f', "..-." },
    { 'g', "--." },
    { 'h', "...." },
    { 'i', ".." },
    { 'j', ".---" },
    { 'k', "-.-" },
    { 'l', ".-.." },
    { 'm', "--" },
    { 'n', "-." },
    { 'o', "---" },
    { 'p', ".--." },
    { 'q', "--.-" },
    { 'r', ".-." },
    { 's', "..." },
    { 't', "-" },
    { 'u', "..-" },
    { 'v', "...-" },
    { 'w', ".--" },
    { 'x', "-..-" },
    { 'y', "-.--" },
    { 'z', "--.." },
    { ' ', " " },
    { '0', "-----" },
    { '1', ".----" },
    { '2', "..---" },
    { '3', "...--" },
    { '4', "....-" },
    { '5', "....." },
    { '6', "-...." },
    { '7', "--..." },
    { '8', "---.." },
    { '9', "----." }
};
static int nchars = sizeof(code) / sizeof (struct Morse);

// how fast do you want to go?
// a value of 300000 takes about a minute to say "raspberry alpha omega"
// smaller numbers make it faster
#define dot_pause 150000

#define dash_pause (dot_pause * 3)
#define gap_pause dot_pause
#define letter_pause (dot_pause * 3)
#define word_pause (gap_pause * 7)

static switch_fn switcher = &raspi_okled_set;

void morse_set_switch(switch_fn fn) {
  switcher = fn;
}

void switch_off(int duration) {
  switcher(GPIO_LOW);
  raspi_timer_wait(duration);
}

void switch_on(int duration) {
  switcher(GPIO_HIGH);
  raspi_timer_wait(duration);
}

void morse_glyph(char c) {
  switch(c) {
  case '.':
    switch_on(dot_pause);
    break;
  case '-':
    switch_on(dash_pause);
    break;
  default:
    raspi_timer_wait(word_pause);
    break;
  }

  switch_off(gap_pause);
}

void morse_symbol(const char* symbol) {
  struct Crate* crate;

  for (char c = *symbol; c != 0; c = *++symbol) {
    morse_glyph(c);
  }

  raspi_timer_wait(letter_pause);
}


void morse_char(char c) {
  for (int i = 0; i < nchars; ++i) {
    if (c == code[i].letter) {
      morse_symbol(code[i].symbol);
      return;
    }
  }
}

void morse_string(const char* message) {
  for (char c = *message; c != 0; c = *++message) {
    morse_char(c);
  }
}

void flash_dots(int ndots) {
  for (int i = 0; i < ndots; ++i) {
    morse_glyph('.');
  }

  raspi_timer_wait(letter_pause);
}

void halt(const char* message) {
  for (;;) {
    morse_string(message);
  }
}
