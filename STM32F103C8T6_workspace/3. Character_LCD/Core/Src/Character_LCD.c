/*
 * Character_LCD.c
 *
 *  Created on: Mar 21, 2022
 *      Author: SangDon Park
 *
 *  Datasheet : HD44780U(LCD-2)
 *             (Doc Matrix Liquid Crystal Display Controller/Driver)
 *
 */

#include <string.h>
#include "Character_LCD.h"

static CLCD_EMS	ems_ctrl;
static CLCD_DOC	doc_ctrl;
static CLCD_CODS	cods_ctrl;
static const CLCD_PIN* 	clcd_pin;

static void CLCD_Pin_Set_Exec(CLCD_PIN_S clcd_pin);
static void CLCD_GPIO_Set(CLCD_PIN_S select_pin);
static void CLCD_Config_Init();
static void CLCD_Inst_Exec(void);
static void CLCD_Entry_Mode_Set(CLCD_EMS_E select);
static void CLCD_Display_ON_OFF_Control(CLCD_DOC_E select);
static void CLCD_Cursor_Or_Display_Shift(CLCD_CODS_E select);
static void CLCD_Function_Set(void);
static void CLCD_Set_CGRAM_Address(void);
static void CLCD_Set_DDRAM_address(uint16_t row, uint16_t col);
static void CLCD_Read_Busy_Flag_And_Address(void);
static void CLCD_Write_Data_To_CG_OR_DDRAM(uint16_t data);
static void CLCD_Read_Data_From_CG_OR_DDRAM(void);

static void CLCD_Pin_Set_Exec(CLCD_PIN_S clcd_pin)
{
#if CLCD_I_FS_D_L == 1
	CLCD_GPIO_Set(clcd_pin);
	CLCD_Inst_Exec();
#else
	CLCD_PIN_S tmp_pin;
	CLCD_GPIO_Set(clcd_pin);
	CLCD_Inst_Exec();

	tmp_pin = (clcd_pin & 0x00f) << 4;
	clcd_pin &= 0x600;
	clcd_pin |= tmp_pin;
	CLCD_GPIO_Set(clcd_pin);
	CLCD_Inst_Exec();
#endif
}

static void CLCD_GPIO_Set(CLCD_PIN_S select_pin)
{
	int16_t i;
#if CLCD_I_FS_D_L == 1
	for(i = 10; i >= 0; i--)
#else
	select_pin = select_pin >> 4;
	for(i = 6; i >= 0; i--)
#endif
	{
		if((select_pin >> i) & 0x001)
			HAL_GPIO_WritePin(clcd_pin[i].lcd_gpio_type, clcd_pin[i].pin_num, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(clcd_pin[i].lcd_gpio_type, clcd_pin[i].pin_num, GPIO_PIN_RESET);
	}
}

static void CLCD_Config_Init()
{
	static const CLCD_PIN clcd_pin_sc[] = {
#if CLCD_I_FS_D_L
		{ CLCD_PIN_D0_TYPE, CLCD_PIN_D0_NUM },	// idx =  0
		{ CLCD_PIN_D1_TYPE, CLCD_PIN_D1_NUM },  // idx =  1
		{ CLCD_PIN_D2_TYPE, CLCD_PIN_D2_NUM },  // idx =  2
		{ CLCD_PIN_D3_TYPE, CLCD_PIN_D3_NUM },  // idx =  3
#endif
		{ CLCD_PIN_D4_TYPE, CLCD_PIN_D4_NUM },  // idx =  4, 0
		{ CLCD_PIN_D5_TYPE, CLCD_PIN_D5_NUM },  // idx =  5, 1
		{ CLCD_PIN_D6_TYPE, CLCD_PIN_D6_NUM },  // idx =  6, 2
		{ CLCD_PIN_D7_TYPE, CLCD_PIN_D7_NUM },  // idx =  7, 3
		{ CLCD_PIN_E_TYPE , CLCD_PIN_E_NUM  },  // idx =  8, 4
		{ CLCD_PIN_RW_TYPE, CLCD_PIN_RW_NUM },	// idx =  9, 5
		{ CLCD_PIN_RS_TYPE, CLCD_PIN_RS_NUM },  // idx = 10, 6
	};

	clcd_pin = clcd_pin_sc;

	ems_ctrl.i_d = CLCD_I_EMS_I_D;
	ems_ctrl.s = CLCD_I_EMS_S;

	doc_ctrl.d = CLCD_I_DOC_D;
	doc_ctrl.c = CLCD_I_DOC_C;
	doc_ctrl.b = CLCD_I_DOC_B;

	cods_ctrl.s_c = CLCD_I_CODS_S_C;
	cods_ctrl.r_l = CLCD_I_CODS_R_L;
}

static void CLCD_Inst_Exec(void)
{
	HAL_GPIO_WritePin(CLCD_PIN_E_TYPE, CLCD_PIN_E_NUM, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(CLCD_PIN_E_TYPE, CLCD_PIN_E_NUM, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void CLCD_Init(void)
{
	CLCD_Config_Init();
	HAL_Delay(40);
	CLCD_Pin_Set_Exec(CLCD_PIN_S_DB5 | CLCD_PIN_S_DB4);
	HAL_Delay(5);
	CLCD_Pin_Set_Exec(CLCD_PIN_S_DB5 | CLCD_PIN_S_DB4);
	HAL_Delay(1);
	CLCD_Pin_Set_Exec(CLCD_PIN_S_DB5 | CLCD_PIN_S_DB4);
#if CLCD_I_FS_D_L == 0
	CLCD_GPIO_Set(CLCD_PIN_S_DB5);
	CLCD_Inst_Exec();
#endif
	CLCD_Function_Set();
	CLCD_Display_ON_OFF_Control(CLCD_DOC_E_NONE);
	CLCD_Clear_Display();
	CLCD_Entry_Mode_Set(CLCD_EMS_E_INIT);
	// Initialization Ends

	CLCD_Display_ON_OFF_Control(CLCD_DOC_E_INIT);
}

void CLCD_Clear_Display(void)
{
	CLCD_Pin_Set_Exec(CLCD_PIN_S_DB0);
}

void CLCD_Return_Home(void)
{
	CLCD_Pin_Set_Exec(CLCD_PIN_S_DB1);
	HAL_Delay(1);
}

static void CLCD_Entry_Mode_Set(CLCD_EMS_E select)
{
	uint16_t clcd_pin = CLCD_PIN_S_DB3;

	ems_ctrl.bits = select;
	clcd_pin |= CLCD_PIN_S_DB2;
	clcd_pin |= (ems_ctrl.i_d ? CLCD_PIN_S_DB1 : 0);
	clcd_pin |= (ems_ctrl.s ? CLCD_PIN_S_DB0 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

static void CLCD_Display_ON_OFF_Control(CLCD_DOC_E select)
{
	uint16_t clcd_pin = CLCD_PIN_S_DB3;

	doc_ctrl.bits = select;
	clcd_pin |= CLCD_PIN_S_DB3;
	clcd_pin |= (doc_ctrl.d ? CLCD_PIN_S_DB2 : 0);
	clcd_pin |= (doc_ctrl.c ? CLCD_PIN_S_DB1 : 0);
	clcd_pin |= (doc_ctrl.b ? CLCD_PIN_S_DB0 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

static void CLCD_Cursor_Or_Display_Shift(CLCD_CODS_E select)
{
	uint16_t clcd_pin = CLCD_PIN_S_DB3;

	cods_ctrl.bits = select;
	clcd_pin |= CLCD_PIN_S_DB4;
	clcd_pin |= (cods_ctrl.s_c ? CLCD_PIN_S_DB3 : 0);
	clcd_pin |= (cods_ctrl.r_l ? CLCD_PIN_S_DB2 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

static void CLCD_Function_Set(void)
{
	uint16_t clcd_pin = 0;

	clcd_pin |= CLCD_PIN_S_DB5;
	clcd_pin |= (CLCD_I_FS_D_L ? CLCD_PIN_S_DB4 : 0);
	clcd_pin |= (CLCD_I_FS_N ? CLCD_PIN_S_DB3 : 0);
	clcd_pin |= (CLCD_I_FS_F ? CLCD_PIN_S_DB2 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

static void CLCD_Set_CGRAM_Address(void)
{

}

static void CLCD_Set_DDRAM_address(uint16_t row, uint16_t col)
{
	uint16_t pin_s = CLCD_PIN_S_DB7;

#if CLCD_I_FS_N == 1
	if(row)
		row = 0x40;
	pin_s |= (row | col);
#else
	if(row)
		return;
	else
		pin_s |= col;
	}
#endif
	CLCD_Pin_Set_Exec(pin_s);
}

static void CLCD_Read_Busy_Flag_And_Address(void)
{

}

static void CLCD_Write_Data_To_CG_OR_DDRAM(uint16_t data)
{
	uint16_t pin_s = CLCD_PIN_S_RS;
	pin_s |= data;
	CLCD_Pin_Set_Exec(pin_s);
}

static void CLCD_Read_Data_From_CG_OR_DDRAM(void)
{

}

void CLCD_Write(uint16_t row, uint16_t col, const int8_t* str)
{
	int16_t i;
	int32_t str_size;
	str_size = strlen(str);

	if(str_size > 16)
		str_size = 16;

	CLCD_Set_DDRAM_address(row, col);

	for(i = 0; i < str_size; i++)
		CLCD_Write_Data_To_CG_OR_DDRAM(str[i]);
}

