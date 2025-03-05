#include "communication.h"

RS485 rs485;
uint8_t TX3_busy_Flag = 0;

/**
 * @brief	串口2调用结构体 rs485 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Send_Statu_Init( void )
{
    rs485.TX2_busy_Flag = 0;
    rs485.RX2_rev_end_Flag = 0;
    rs485.TX2_buf[128] = 0;
    rs485.RX2_buf[128] = 0;
    rs485.TX2_send_bytelength = 0;
    rs485.TX2_send_cnt = 0;
    rs485.RX2_rev_timeout = 0;
    rs485.RX2_rev_cnt = 0;
    DR2 = 0;
    rs485.send_scan_flag = 0;
}

/**
 * @brief	串口2中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart2_ISR() interrupt 8 
{   
    /* 1, 检测到硬件将S2TI置1，即发送完毕                       */
    if( S2CON & S2TI )          //
    {
        /* 2, 软件将S2TI清零，等待发送标志位重置，可继续发送    */
        S2CON &= ~S2TI;         
        rs485.TX2_busy_Flag = 0;
        
        /* 3, 依次将TX2_buf中数据送出（写S2BUF操作即为发送）    */
        if( rs485.TX2_send_bytelength != 0 )
        {
            S2BUF = rs485.TX2_buf[rs485.TX2_send_cnt++];
            rs485.TX2_send_bytelength--;
        }else
        {
            rs485.TX2_send_cnt = 0;
            DR2 = 0;
        }
    }
    
    /* 1, 检测到硬件将S2RI置1，即接收完毕                       */
    if( S2CON & S2RI )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        S2CON &= ~S2RI;

        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485.RX2_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485.RX2_rev_cnt > 128 )
            {
                rs485.RX2_rev_cnt = 0;
            }

            /* 5, 依次将RX2_buf中数据接收（读S2BUF操作即为接收）*/
            rs485.RX2_buf[rs485.RX2_rev_cnt] = S2BUF;
            rs485.RX2_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485.RX2_rev_timeout = 50;
    }
}

/**
 * @brief	是否接收完毕判断函数
 *
 * @param   
 *
 * @return  void
**/
void Tim0_ISR( void ) interrupt 1   //1ms
{
    /* 1, 如果接收未超时                                             */
    if ( rs485.RX2_rev_timeout != 0 )  
    {
        rs485.RX2_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485.RX2_rev_timeout == 0 )  
        {
            if( rs485.RX2_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485.RX2_rev_end_Flag = 1;    
            }
        }
    } 
}


/**
 * @brief	串口3中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart3_ISR() interrupt 17
{
    if (S3CON & S3TI)                //在停止位开始发送时，该位置1
    {
        S3CON &= ~S3TI;   			     //清除S3CON寄存器对应S3TI位（该位必须软件清零）
        TX3_busy_Flag = 0;
    }

    if (S3CON & S3RI)                //串行接收到停止位的中间时刻时，该位置1
    {
        S3CON &= ~S3RI;              //清除S3CON寄存器对应S3RI位（该位必须软件清零） 
    }
}

// /**
//  * @brief	串口3发送1字节数据
//  *
//  * @param   
//  *
//  * @return  void
// **/
// void Uart3_Sendbyte( uint8_t dat )
// {
//     while( TX3_busy_Flag );
//     TX3_busy_Flag = 1;
//     S3BUF = dat;
// }

// /**
//  * @brief	串口3发送字符串
//  *
//  * @param   
//  *
//  * @return  void
// **/
// void Uart3_SendStr( uint8_t *sendstr )
// {   
//     while(*sendstr)
//     {
//         Uart3_Sendbyte(*sendstr++);
//     }
// }

/**
 * @brief	串口重定向
 *
 * @param   c:字符串
 *
 * @return  c
**/
char putchar(char c)  // 串口重定向需要添加头文件stdio.h
{
    S3BUF = c;
    while(TX3_busy_Flag);
    TX3_busy_Flag = 1;
    return c;
}


