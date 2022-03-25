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

#include "Character_LCD.h"

static CLCD_EMS	ems_ctrl;
static CLCD_DOC	doc_ctrl;
static CLCD_CODS	cods_ctrl;
static const CLCD_FS*	fs_ctrl;
static CLCD_PIN* 	clcd_pin;

void CLCD_Pin_Set_Exec(uint16_t clcd_pin)
{
	//	---- 0000 0000 0000
	int16_t last_pin_idx;
	uint16_t tmp_pin;

	if(fs_ctrl->d_l)
	{
		last_pin_idx = 0;
		CLCD_GPIO_Set(clcd_pin, last_pin_idx);
		CLCD_Inst_Exec();
	}
	else
	{
		last_pin_idx = 4;
		CLCD_GPIO_Set(clcd_pin, last_pin_idx);
		CLCD_Inst_Exec();
		if(clcd_pin & CLCD_PIN_S_4_BIT_OP_ONCE)
			return;
		else
		{
			tmp_pin = (clcd_pin & 0x00f) << 4;
			clcd_pin &= 0x600;
			clcd_pin |= tmp_pin;
			CLCD_GPIO_Set(clcd_pin, last_pin_idx);
			CLCD_Inst_Exec();
		}
	}
}

void CLCD_GPIO_Set(uint16_t select_pin, int16_t last_pin_idx)
{
	int16_t i;
	int16_t start_pin_idx = 10;

	for(i = start_pin_idx; i >= last_pin_idx; i--)
	{
		if((select_pin >> i) & 0x001)
		{
			HAL_GPIO_WritePin(clcd_pin[i].lcd_gpio_type, clcd_pin[i].pin_num, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(clcd_pin[i].lcd_gpio_type, clcd_pin[i].pin_num, GPIO_PIN_RESET);
		}
	}
}

void CLCD_Config_Init()
{
	static const CLCD_PIN clcd_pin_sc[] = {
			{ CLCD_PIN_D0_TYPE, CLCD_PIN_D0_NUM },	// idx =  0
			{ CLCD_PIN_D1_TYPE, CLCD_PIN_D1_NUM },  // idx =  1
			{ CLCD_PIN_D2_TYPE, CLCD_PIN_D2_NUM },  // idx =  2
			{ CLCD_PIN_D3_TYPE, CLCD_PIN_D3_NUM },  // idx =  3
			{ CLCD_PIN_D4_TYPE, CLCD_PIN_D4_NUM },  // idx =  4
			{ CLCD_PIN_D5_TYPE, CLCD_PIN_D5_NUM },  // idx =  5
			{ CLCD_PIN_D6_TYPE, CLCD_PIN_D6_NUM },  // idx =  6
			{ CLCD_PIN_D7_TYPE, CLCD_PIN_D7_NUM },  // idx =  7
			{ CLCD_PIN_E_TYPE, 	CLCD_PIN_E_NUM 	},  // idx =  8
			{ CLCD_PIN_RW_TYPE, CLCD_PIN_RW_NUM },	// idx =  9
			{ CLCD_PIN_RS_TYPE, CLCD_PIN_RS_NUM }   // idx = 10
	};

	clcd_pin = clcd_pin_sc;

	ems_ctrl.i_d = CLCD_I_EMS_I_D;
	ems_ctrl.s = CLCD_I_EMS_S;

	doc_ctrl.d = CLCD_I_DOC_D;
	doc_ctrl.c = CLCD_I_DOC_C;
	doc_ctrl.b = CLCD_I_DOC_B;

	cods_ctrl.s_c = CLCD_I_CODS_S_C;
	cods_ctrl.r_l = CLCD_I_CODS_R_L;

	static const CLCD_FS fs_ctrl_v = {
		CLCD_I_FS_D_L,
		CLCD_I_FS_N,
		CLCD_I_FS_F
	};
	fs_ctrl = &fs_ctrl_v;
}

void CLCD_Inst_Exec(void)
{
	uint16_t e_idx = 8;
	HAL_GPIO_WritePin(clcd_pin[e_idx].lcd_gpio_type, clcd_pin[e_idx].pin_num, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(clcd_pin[e_idx].lcd_gpio_type, clcd_pin[e_idx].pin_num, GPIO_PIN_RESET);
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
	CLCD_Pin_Set_Exec(CLCD_PIN_S_4_BIT_OP_ONCE | CLCD_PIN_S_DB5);
	CLCD_Function_Set();
	CLCD_Display_ON_OFF_Control(CLCD_DOC_S_NONE);
	CLCD_Clear_Display();
	CLCD_Entry_Mode_Set(CLCD_EMS_S_I_D);
	// Initialization Ends

	CLCD_Display_ON_OFF_Control(CLCD_DOC_S_D | CLCD_DOC_S_C);
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

void CLCD_Entry_Mode_Set(uint16_t select)
{
	uint16_t clcd_pin = 0;
	uint16_t i = 0;
	uint16_t *p = (uint16_t*)&ems_ctrl;

	for(i = 0; i < 2; i++)
	{
		if(select & ((0x001) << i))
			p[i] = 1;
		else
			p[i] = 0;
	}

	clcd_pin |= CLCD_PIN_S_DB2;
	clcd_pin |= (ems_ctrl.i_d ? CLCD_PIN_S_DB1 : 0);
	clcd_pin |= (ems_ctrl.s ? CLCD_PIN_S_DB0 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

void CLCD_Display_ON_OFF_Control(uint16_t select)
{
	uint16_t clcd_pin = 0;
	uint16_t i = 0;
	uint16_t *p = (uint16_t*)&doc_ctrl;

	for(i = 0; i < 3; i++)
	{
		if(select & ((0x001) << i))
			p[i] = 1;
		else
			p[i] = 0;
	}

	clcd_pin |= CLCD_PIN_S_DB3;
	clcd_pin |= (doc_ctrl.d ? CLCD_PIN_S_DB2 : 0);
	clcd_pin |= (doc_ctrl.c ? CLCD_PIN_S_DB1 : 0);
	clcd_pin |= (doc_ctrl.b ? CLCD_PIN_S_DB0 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

void CLCD_Cursor_Or_Display_Shift(uint16_t select)
{
	uint16_t clcd_pin = 0;
	uint16_t i;
	uint16_t *p = (uint16_t*)&cods_ctrl;

	for(i = 0; i < 2; i++)
	{
		if(select & ((0x001) << i))
			p[i] = 1;
		else
			p[i] = 0;
	}

	clcd_pin |= CLCD_PIN_S_DB4;
	clcd_pin |= (cods_ctrl.s_c ? CLCD_PIN_S_DB3 : 0);
	clcd_pin |= (cods_ctrl.r_l ? CLCD_PIN_S_DB2 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

void CLCD_Function_Set(void)
{
	uint16_t clcd_pin = 0;

	clcd_pin |= CLCD_PIN_S_DB5;
	clcd_pin |= (fs_ctrl->d_l ? CLCD_PIN_S_DB4 : 0);
	clcd_pin |= (fs_ctrl->n ? CLCD_PIN_S_DB3 : 0);
	clcd_pin |= (fs_ctrl->f ? CLCD_PIN_S_DB2 : 0);

	CLCD_Pin_Set_Exec(clcd_pin);
}

void CLCD_Set_CGRAM_Address(void)
{

}

void CLCD_Read_Busy_Flag_And_Address(void)
{

}

void CLCD_Write_Data_To_CG_OR_DDRAM(void)
{
	CLCD_Pin_Set_Exec(CLCD_PIN_S_RS | CLCD_PIN_S_DB6 | CLCD_PIN_S_DB3);
}

void CLCD_Read_Data_From_CG_OR_DDRAM(void)
{

}


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

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01000101(E)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01001100(L)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01001100(L)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01001111(O)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 10100000(space)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01010111(W)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01001111(O)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01010010(R)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01001100(L)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 01000100(D)
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // DB6 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 00100001(!)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

	//////////////////////////////////////////////////////////////////////////
	// Write data = 00100001(!)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // DB5 = 1
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
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // DB4 = 1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0
}

void Set_DDRRAM_address(void)
{
	// 001(ADD)(ADD)(ADD)
	// 00(ADD)(ADD)(ADD)(ADD)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DB7 = 1
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
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // DB7 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // DB6 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // DB5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // DB4 = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // E = 0

}
