#include "gpio.h"

int main(void) {
	gpio_init();

	while(1) {
		raspi_okled_set(1);
		raspi_timer_wait(500000);

		raspi_okled_set(0);
		raspi_timer_wait(500000);
	}
} 
