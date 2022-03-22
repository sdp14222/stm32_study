/*
 * Character_LCD.h
 *
 *  Created on: Mar 21, 2022
 *      Author: SangDon Park
 */

#ifndef INC_CHARACTER_LCD_H_
#define INC_CHARACTER_LCD_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

void Character_LCD_init(void);
void Display_clear(void);
void Function_set(void);
void Display_on_off_control(void);
void Entry_mode_set(void);
void Write_data_to_CGRAM_DDRAM(void);

#endif /* INC_CHARACTER_LCD_H_ */
