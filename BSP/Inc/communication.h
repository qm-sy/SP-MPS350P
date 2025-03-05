#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "uart.h"
#include "stdio.h"
#include "gpio.h"

typedef struct 
{
    uint8_t     TX2_busy_Flag;          //等待发送标志位
    uint8_t     RX2_rev_end_Flag;       //数据包接收完毕标志
    uint8_t     TX2_buf[128];           //SBUF TI缓冲区
    uint8_t     RX2_buf[128];           //SBUF RI缓冲区
    uint8_t     TX2_send_bytelength;    //发送字节数
    uint8_t     TX2_send_cnt;           //发送计数
    uint16_t    RX2_rev_timeout;        //接收超时
    uint8_t     RX2_rev_cnt;            //接收计数
    uint8_t     DR_Flag;                //DR
    uint8_t     send_scan_flag;
}RS485;

extern RS485 rs485;

void Uart2_Sendbyte( uint8_t dat );
void Uart2_SendStr( uint8_t *sendstr );
void Uart2_Send_Statu_Init( void );

void Uart3_Sendbyte( uint8_t dat );
void Uart3_SendStr( uint8_t *sendstr );
char putchar(char c);

#endif