/*
 * Character_LCD.c
 *
 *  Created on: Mar 21, 2022
 *      Author: SangDon Park
 */

#include "Character_LCD.h"

static void Reset_LCD_func(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

void Character_LCD_init(void)
{
	HAL_Delay(15);

	// 000011
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_Delay(5);

	// 000011
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_Delay(1);

	// 000011
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// 000010
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// 00001(DL)
	// 00NF**
	// DL = 0; Sets the interface data length. 8-bit lengths = 1, 4-bit lengths = 0
	// N = 1; 2-line display
	// F = 0; 5x8 dot character font)
	// Function Set
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// 000000
	// 001DCB
	// D = 0; Display off
	// C = 0; Cursor off
	// B = 0; Blink off
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// 000000
	// 000001
	// Display Clear
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// 000000
	// 0001(I/D)S
	// I = 1; Increment by 1
	// S = 0; No shift
	// Entry Mode Set
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

void Display_clear(void)
{
//	Reset_LCD_func();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

}

void Function_set(void)
{
//	Reset_LCD_func();

	// 4-bit operation
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = *
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = *
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	// Function set
	// 00001(DL)
	// 00NF**
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = *
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = *
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

}

void Display_on_off_control(void)
{
//	Reset_LCD_func();
	// 000000
	// 001DCB
	// D = 1; Display On
	// C = 1; Cursor On
	// B = 0; Blink Off
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // DB3 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // DB2 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // DB1 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

//	HAL_Delay(1000);
//	Reset_LCD_func();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

void Entry_mode_set(void)
{
//	Reset_LCD_func();
	// 000000
	// 0001(I/D)S
	// I = 1; Increment by 1
	// S = 0; No shift
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DB3 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // DB2 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // DB1 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

//	HAL_Delay(1000);
//	Reset_LCD_func();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

void Write_data_to_CGRAM_DDRAM(void)
{
//	Reset_LCD_func();
	// 10(Write data)
	// 10(Write data)
	// Write data = 01001000(H)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // DB3 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

//	HAL_Delay(1);
//	Reset_LCD_func();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	/*
	//////////////////////////////////////////////////////////////////////
	// Write data = 01001001(I)
	yHAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // DB3 = 1
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

//	HAL_Delay(1000);
//	Reset_LCD_func();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
	*/

	//////////////////////////////////////////////////////////////////////
	// Set DDRAM address
	// 001100
	// 000000
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // DB3 = 1
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
//
////	HAL_Delay(1000);
////	Reset_LCD_func();
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET); // E = 1
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0


	//////////////////////////////////////////////////////////////////////
	// Return Home
	//	 000000
	//	 00001-
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // DB3 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // DB2 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // DB1 = 0
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // DB0 = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
//
////	HAL_Delay(1000);
////	Reset_LCD_func();
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET); // E = 1
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

