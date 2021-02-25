#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__


#include "ht1621b.h"
#include "flash.h"

#define BIAS		0X52 	//ƫ�õ�ѹ
#define	SYSDIS		0X00	//�ر�ϵͳʱ�Ӻ�ƫѹ������
#define SYSEN   	0X02	//��ϵͳʱ��
#define LCD_OFF		0X04	//�ر�ƫѹ������
#define LCD_ON		0X06	//��ƫѹ������
#define RC256 		0X30	//�ڲ�ʱ��256K
#define TONEON 		0X12 	//���������
#define TONEOFF 	0X10  	//�ر��������
#define WDTDIS 		0X0A 	//��ֹ���Ź�
#define XTAL_32K	0X28	//�ⲿ��ʱ��32k  
#define TMDIS		0x08

#define COM1 0X01
#define COM2 0X02
#define COM3 0X04
#define COM4 0X08
#define COM_OFF 0x00
#define SET    1


typedef enum
{
 NO_EORROR = 0,
 F_FLASH = 1,
 E_FLASH = 2
    
}EORROR_Index;

typedef enum
{
 
 Gap_30_Pin = 30,
 Gap_29_Pin = 29,
 Time_28_Pin = 28,
 Time_27_Pin = 27,
 Time_31_Pin = 31
}SEG_TUBE;

typedef struct _time_var
{
  u8 reflish_std:4;
  u8 blink_std:4;  
  u8 time;
  u8 gap;    
}var_info_t;

extern var_info_t Var_info_display;

void lcd_display_time(u8 time_type);
void lcd_display_gap(u8 gap);

void init_lcd_ht1621b(void);
void ht1621_all_display(void);
void ht1621_all_clear(void);


void lcd_display_blink_set(EORROR_Index index);
void display_gap_process(EORROR_Index index);
void display_process(void);

#endif
