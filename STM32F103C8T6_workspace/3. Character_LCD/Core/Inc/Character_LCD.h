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


/********* CLCD_GPIO_PIN_CONFIG *********/
#define CLCD_PIN_RS_TYPE	GPIOC
#define CLCD_PIN_RS_NUM		GPIO_PIN_13
#define CLCD_PIN_RW_TYPE	GPIOC
#define CLCD_PIN_RW_NUM		GPIO_PIN_14
#define CLCD_PIN_E_TYPE		GPIOC
#define CLCD_PIN_E_NUM		GPIO_PIN_15

#define CLCD_PIN_D7_TYPE	GPIOB
#define CLCD_PIN_D7_NUM		GPIO_PIN_4
#define CLCD_PIN_D6_TYPE	GPIOB
#define CLCD_PIN_D6_NUM		GPIO_PIN_3
#define CLCD_PIN_D5_TYPE	GPIOB
#define CLCD_PIN_D5_NUM		GPIO_PIN_1
#define CLCD_PIN_D4_TYPE	GPIOB
#define CLCD_PIN_D4_NUM		GPIO_PIN_0

#define CLCD_PIN_D3_TYPE	GPIOB
#define CLCD_PIN_D3_NUM		GPIO_PIN_8
#define CLCD_PIN_D2_TYPE	GPIOB
#define CLCD_PIN_D2_NUM		GPIO_PIN_7
#define CLCD_PIN_D1_TYPE	GPIOB
#define CLCD_PIN_D1_NUM		GPIO_PIN_6
#define CLCD_PIN_D0_TYPE	GPIOB
#define CLCD_PIN_D0_NUM		GPIO_PIN_5


/********* INSTRUCTIONS_INIT_CONFIG *********/

/********* ENTRY_MODE_SET_INIT_CONFIG *********/
#define CLCD_I_EMS_I_D	1		// 0 = DECREMENT,	1 = INCREMENT
#define CLCD_I_EMS_S	0		// 0 = No Shift,	1 = Accompanies display shift

/********* DISPLAY_ON_OFF_INIT_CONFIG *********/
#define CLCD_I_DOC_D	0		// 0 = Display off,		1 = Display on
#define CLCD_I_DOC_C	0		// 0 = Cursor off,		1 = Cursor on
#define CLCD_I_DOC_B	0		// 0 = Blinking off,	1 = Blinking on

/********* CURSOR_OR_DISPLAY_SHIFTL_INIT_CONFIG *********/
#define CLCD_I_CODS_S_C	0		// 0 = Cursor move,			1 = Display shift
#define CLCD_I_CODS_R_L	0		// 0 = Shift to the left,	1 = Shift to the left

/********* FUNTION_SET_INIT_CONFIG *********/
#define CLCD_I_FS_D_L	0		// 0 = 4 bits,		1 = 8 bits
#define CLCD_I_FS_N		1		// 0 = 1 line,		1 = 2 lines
#define CLCD_I_FS_F		0		// 0 = 5x8 dots,	1 = 5x10 dots


typedef struct
{
	uint16_t i_d;
	uint16_t s;
} CLCD_EMS;

typedef struct
{
	uint16_t d;
	uint16_t c;
	uint16_t b;
} CLCD_DOC;

typedef struct
{
	uint16_t s_c;
	uint16_t r_l;
} CLCD_CODS;

typedef struct
{
	uint16_t d_l;
	uint16_t n;
	uint16_t f;
} CLCD_FS;

typedef struct
{
	GPIO_TypeDef* lcd_gpio_type;
	uint16_t pin_num;
} CLCD_PIN;


/********* CLCD_PIN_SELECT *********/
#define CLCD_PIN_S_4_BIT_OP_ONCE	((uint16_t)0x800)
#define CLCD_PIN_S_RS				((uint16_t)0x400)
#define CLCD_PIN_S_RW				((uint16_t)0x200)
#define CLCD_PIN_S_E				((uint16_t)0x100)
#define CLCD_PIN_S_DB7				((uint16_t)0x080)
#define CLCD_PIN_S_DB6				((uint16_t)0x040)
#define CLCD_PIN_S_DB5				((uint16_t)0x020)
#define CLCD_PIN_S_DB4				((uint16_t)0x010)
#define CLCD_PIN_S_DB3				((uint16_t)0x008)
#define CLCD_PIN_S_DB2				((uint16_t)0x004)
#define CLCD_PIN_S_DB1				((uint16_t)0x002)
#define CLCD_PIN_S_DB0				((uint16_t)0x001)
#define CLCD_PIN_S_ALL_SET			((uint16_t)0x7FF)
#define CLCD_PIN_S_ALL_RESET		((uint16_t)0x000)

/********* CLCD_Entry_Mode_Set_SELECT *********/
#define CLCD_EMS_S_I_D				((uint16_t)0x001)
#define CLCD_EMS_S_S				((uint16_t)0x002)
#define CLCD_EMS_S_ALL				((uint16_t)0x003)
#define CLCD_EMS_S_NONE				((uint16_t)0x000)

/********* CLCD_Display_ON_OFF_Control_SELECT *********/
#define CLCD_DOC_S_D				((uint16_t)0x001)
#define CLCD_DOC_S_C				((uint16_t)0x002)
#define CLCD_DOC_S_B				((uint16_t)0x004)
#define CLCD_DOC_S_ALL				((uint16_t)0x007)
#define CLCD_DOC_S_NONE				((uint16_t)0x000)

/********* CLCD_Cursor_Or_Display_Shift_SELECT *********/
#define CLCD_CODS_S_S_C				((uint16_t)0x001)
#define CLCD_CODS_S_R_L				((uint16_t)0x002)
#define CLCD_CODS_S_ALL				((uint16_t)0x003)
#define CLCD_CODS_S_NONE			((uint16_t)0x000)



void CLCD_Init(void);
void CLCD_Clear_Display(void);
void CLCD_Return_Home(void);
void CLCD_Entry_Mode_Set(uint16_t select);
void CLCD_Display_ON_OFF_Control(uint16_t select);
void CLCD_Cursor_Or_Display_Shift(uint16_t select);
void CLCD_Function_Set(void);
void CLCD_Set_CGRAM_Address(void);
void CLCD_Set_DDRAM_address(uint16_t row, uint16_t col);
void CLCD_Read_Busy_Flag_And_Address(void);
void CLCD_Write_Data_To_CG_OR_DDRAM(uint16_t addr);
void CLCD_Read_Data_From_CG_OR_DDRAM(void);
void CLCD_Write(uint16_t row, uint16_t col, const uint8_t* str);



void Character_LCD_init(void);
void Display_clear(void);
void Function_set(void);
void Display_on_off_control(void);
void Entry_mode_set(void);
void Write_data_to_CGRAM_DDRAM(void);
void Set_DDRRAM_address(void);

#endif /* INC_CHARACTER_LCD_H_ */
