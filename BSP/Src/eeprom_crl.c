#include "eeprom_crl.h"

EEPROM_INFO eeprom;

/**
 * @brief	eeprom状态判定，是否写入过
 *
 * @param   
 *
 * @return  void
**/
void eeprom_statu_judge( void )
{
    uint8_t eeprom_statu_flag;

    eeprom_statu_flag = ISP_Read(EEPROM_STATU_JUDGE);
    //printf("The value of eeprom_statu_flag is 0x%02x \r\n",(int)eeprom_statu_flag);
    if( eeprom_statu_flag == 0xFF)
    {
        eeprom.fan_info          = 0x03;          //011 pwm7默认开3档风速
        eeprom.led_switch        = 0x01;          //0000000 1  led默认开
        eeprom.ac220_switch      = 0x00;          //默认关闭220V输出
        eeprom.bake_power_level  = 0x32;          //220V_CH4默认开，50%功率
        eeprom.bake_fan_level    = 0x03;          //011 pwm8默认开3档风速  
        eeprom.temp_alarm_value1 = 0x50;          //NTC1 alarm value 默认80℃
        eeprom.temp_alarm_value2 = 0x50;          //NTC2 alarm value 默认80℃  
        eeprom.temp_alarm_value3 = 0x50;          //NTC3 alarm value 默认80℃ 

        eeprom_data_record(); 
    }
    eeprom_data_init();    
}

/**
 * @brief	eeprom 数据写入
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_record( void )
{
    ISP_Earse(0x0000);

    ISP_Write(FAN_ADDR,eeprom.fan_info);
    ISP_Write(LED_ADDR,eeprom.led_switch);
    ISP_Write(AC_SWITCH_ADDR,eeprom.ac220_switch);
    ISP_Write(BAKE_POWER_ADDR,eeprom.bake_power_level);
    ISP_Write(BAKE_FAN_ADDR,eeprom.bake_fan_level);
    ISP_Write(TEMP_ALARM1,eeprom.temp_alarm_value1);
    ISP_Write(TEMP_ALARM2,eeprom.temp_alarm_value2);
    ISP_Write(TEMP_ALARM3,eeprom.temp_alarm_value3);

    ISP_Write(EEPROM_STATU_JUDGE,0x58);
}

/**
 * @brief	eeprom 数据初始化
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_init( void )
{
    /*    PWM7、PWM8 风速及开关状态初始化    */
    eeprom.fan_info = ISP_Read(FAN_ADDR);
    PWMB_CCR7 = eeprom.fan_info *184;

    /*    LED开关状态初始化    */
    eeprom.led_switch = ISP_Read(LED_ADDR);
    if( eeprom.led_switch & 0X01 )
    {
        DC_24V_out(LED_ON);
    }else
    {
        DC_24V_out(LED_OFF);
    }

    /*    220V输出开关状态初始化    */
    eeprom.ac220_switch = ISP_Read(AC_SWITCH_ADDR);
    if( eeprom.ac220_switch & 0X01 )
    {
        INTCLKO |= 0X10;
        temp.temp_scan_allow_flag = 1;
        AC_Out1 = AC_Out2 = AC_Out3 = 0;
    }else
    {
        INTCLKO &= ~0x10;
        temp.temp_scan_allow_flag = 0;
        AC_Out1 = AC_Out2 = AC_Out3 = AC_Out4 =1;
    }

    /*    220V输出开关状态初始化    */
    eeprom.bake_fan_level   = ISP_Read(BAKE_FAN_ADDR);
    eeprom.bake_power_level = ISP_Read(BAKE_POWER_ADDR);

    PWMB_CCR8 = eeprom.bake_fan_level *184;
    ac_220v_crl(eeprom.bake_power_level);

    /*    三路NTC alarm value初始化    */
    eeprom.temp_alarm_value1 = ISP_Read(TEMP_ALARM1);
    eeprom.temp_alarm_value2 = ISP_Read(TEMP_ALARM2);
    eeprom.temp_alarm_value3 = ISP_Read(TEMP_ALARM3);

    temp.temp_alarm_value1 = eeprom.temp_alarm_value1;
    temp.temp_alarm_value2 = eeprom.temp_alarm_value2;
    temp.temp_alarm_value3 = eeprom.temp_alarm_value3;
}
