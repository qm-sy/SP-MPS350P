#include "uart.h"

/**
 * @brief	串口2初始化函数--tim2
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Init( void )       //115200bps@11.0592MHz
{
    P_SW2 |= 0X00;          //串口2功能脚选择P1.0 P1.1

	S2CON  = 0x50;           //8位数据,可变波特率

	AUXR  |= 0x04;           //定时器时钟1T模式

	T2L    = 0xE8;           //设置定时初始值
	T2H    = 0xFF;           //设置定时初始值

	AUXR  |= 0x10;           //定时器2开始计时

	IE2   |= 0X01;			//串口2 中断允许位
}


/**
 * @brief	串口3初始化函数--tim3
 *
 * @param   
 *
 * @return  void
**/
void Uart3_Init( void )	//115200bps@11.0592MHz
{
	P_SW2 |= 0X00;		//串口3功能脚选择P0.0 P0.1
	
	S3CON  = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器

	T4T3M |= 0x02;		//定时器时钟1T模式

	T3L = 0xE8;			//设置定时初始值
	T3H = 0xFF;			//设置定时初始值

	T4T3M |= 0x08;		//定时器3开始计时

	IE2   |= 0x08;		// 串口3中断打开	
}