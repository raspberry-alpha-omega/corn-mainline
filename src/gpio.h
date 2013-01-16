#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define TIMER_BASE (BCM2708_PERI_BASE + 0x3000) /* Timer */

#define GPFSEL1 (GPIO_BASE + 0x04)
#define GPSET0 (GPIO_BASE + 0x1C)
#define GPCLR0 (GPIO_BASE + 0x28)
#define GPPUD (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)

#define TIMER (TIMER_BASE + 0x00)
#define COUNTER (TIMER_BASE + 0x04)

#define UART_BASE (GPIO_BASE + 0x15000)
#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

enum gpio_direction { GPIO_INPUT, GPIO_OUTPUT };
void raspi_set_gpio_direction(int pin, enum gpio_direction direction);

enum gpio_level { GPIO_LOW=0, GPIO_HIGH=1 };
void raspi_set_gpio_level(int pin, enum gpio_level level);

enum gpio_level raspi_get_gpio_level(int pin);

void raspi_okled_set(enum gpio_level level);
void raspi_timer_wait(int usec);
void raspi_mini_uart_send_char(char c);
void raspi_mini_uart_send_string(const char* s);
void raspi_mini_uart_send_hex(uint32_t n);
void raspi_mini_uart_send_newline(void);

void gpio_init();

#endif
