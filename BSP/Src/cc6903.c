#include "cc6903.h"

/**
 * @brief	获取电流值
 *
 * @param   channel_x：电流通道1~3
 *
 * @return  current 
**/
uint16_t get_current( uint8_t channel_x)
{
    uint16_t current;
    uint16_t adc_val = 0;
    uint8_t t;

    /* 1, 取10次adc_val                           */
    for( t = 0; t < 10; t++ )
    {
        adc_val += Get_ADC_12bit(channel_x);
    }
    adc_val = adc_val/10;
    //printf("The  value of adc_val is %d \r\n",(int)adc_val);
    /* 2, 先取余，四舍五入，后对36取整            */
    if( adc_val < 1360 )
    {
        adc_val = 1360;
    }
    current = (adc_val - 1360) % 36;
    if( current >= 18 )
    {
        current  = 1;
    }else
    {
        current = 0;
    }
    current = (adc_val - 1360) / 36 + current;

    if( current > 30 )
    {
        current = 30;
    }
    //printf("The value of current is %d \r\n ",current);
    return current;
}