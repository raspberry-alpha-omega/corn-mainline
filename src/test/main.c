#include <stdio.h>

#include "morse.h"
#include "ledborg.h"

int main() {
  puts("tests go here!");
  morse_set_switch(&ledborg_set_all);
  morse_string("host");
}
