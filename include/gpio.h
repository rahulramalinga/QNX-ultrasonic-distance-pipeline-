
/*
 * gpio.h
 *
 *  Created on: 22-Nov-2025
 *      Author: Rahulr
 */

#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdint.h>
#include <stdbool.h>


#define TRIG_GPIO   23   
#define ECHO_GPIO   24 

int gpio_hal_init(void);
void gpio_hal_deinit(void);

void gpio_set_output(uint32_t gpio);
void gpio_set_input(uint32_t gpio);

void gpio_write(uint32_t gpio, int value);
int  gpio_read(uint32_t gpio);

#endif
