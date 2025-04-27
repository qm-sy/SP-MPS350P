#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"

#define LED_ON    1
#define LED_OFF   0


typedef struct 
{
    uint8_t  ac220_switch;
    uint8_t  ac220_bake_level;
    uint8_t  zero_flag;          //220V输入触发标志位
    uint16_t time_delay;         //移相延时
}AC_220;

extern AC_220 ac_220;


void Power_Statu_Init( void );
void DC_24V_out( uint8_t on_off );
void ac_220v_crl( uint8_t power_level );

#endif