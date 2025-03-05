#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer4：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim4_ISR( void ) interrupt 20
{

    static uint16_t temp_scan_cnt = 0;
    static uint16_t statu_scan_cnt = 0;
    /* 1, 220V AC输出CH1~CH3固定最大功率输出           */

    /* 2, temp 上报 1s/次                             */
    temp_scan_cnt++;
    if( temp_scan_cnt == 100 )
    {
        temp.temp_scan_flag = 1;
        temp_scan_cnt = 0;
    }
    /* 3,  1s/次   发送statu                          */
    if( rs485.send_scan_flag == 0)
    {
        statu_scan_cnt++;
        if( statu_scan_cnt == 100 )
        {
            statu_scan_cnt = 0;
            rs485.send_scan_flag = 1;
        }
    }
}