#ifndef __EEPROM_CRL_H_
#define __EEPROM_CRL_H_

#include "eeprom.h"
#include "sys.h"
#include "power_crl.h"
#include "ntc.h"
#include "stdio.h"

#define FAN_ADDR              0x0000
#define LED_ADDR              0x0001
#define AC_SWITCH_ADDR        0x0002
#define BAKE_POWER_ADDR       0x0003
#define BAKE_FAN_ADDR         0x0004
#define TEMP_ALARM1           0X0005
#define TEMP_ALARM2           0X0006
#define TEMP_ALARM3           0X0007
#define EEPROM_STATU_JUDGE    0X0010         

typedef struct 
{
    uint8_t  fan_info;
    uint8_t  led_switch;         
    uint8_t  ac220_switch;   
    uint8_t  bake_power_level;
    uint8_t  bake_fan_level;
    uint8_t  temp_alarm_value1;        
    uint8_t  temp_alarm_value2;         
    uint8_t  temp_alarm_value3;       
}EEPROM_INFO;

extern EEPROM_INFO eeprom;

void eeprom_statu_judge( void );
void eeprom_data_record( void );
void eeprom_data_init( void );

#endif