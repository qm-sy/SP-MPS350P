#include "cc6903.h"

CURRENT current;

void current_init( void )
{
    current.current1 = 1360;
    current.current2 = 1360;
    current.current3 = 1360;
}
/**
 * @brief	获取电流值
 *
 * @param   channel_x：电流通道1~3
 *
 * @return  current 
**/
uint16_t get_current( uint8_t channel_x)
{
    uint16_t adc_val = 0;
    uint8_t t;

    /* 1, 取10次adc_val                           */
    for( t = 0; t < 15; t++ )
    {
        adc_val += Get_ADC_12bit(channel_x);
    }
    adc_val = adc_val/15;
    
    return adc_val;
}