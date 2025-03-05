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
        eeprom.pwm_info          = 0x33;          //011 011 11 pwm7、8默认开，3档风速
        eeprom.led_info          = 0x01;          //0000000 1  led默认开
        eeprom.ac220_info        = 0x65;          //0110010 1  220V_CH4默认开，50%功率
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

    ISP_Write(PWM_ADDR_EEPROM,eeprom.pwm_info);
    ISP_Write(LED_ADDR_EEPROM,eeprom.led_info);
    ISP_Write(AC220_ADDR_EEPROM,eeprom.ac220_info);
    ISP_Write(TEMP_ALARM1,eeprom.temp_alarm_value1);
    ISP_Write(TEMP_ALARM2,eeprom.temp_alarm_value2);
    ISP_Write(TEMP_ALARM3,eeprom.temp_alarm_value3);

    ISP_Write(EEPROM_STATU_JUDGE,0x58);

    // printf("The value of 0 is : 0x%02x \r\n",(int)eeprom.pwm_info);
    // printf("The value of 1 is : 0x%02x \r\n",(int)eeprom.led_info);
    // printf("The value of 2 is : 0x%02x \r\n",(int)eeprom.ac220_info);
    // printf("The value of 3 is : 0x%02x \r\n",(int)eeprom.temp_alarm_value1);
    // printf("The value of 4 is : 0x%02x \r\n",(int)eeprom.temp_alarm_value2);
    // printf("The value of 5 is : 0x%02x \r\n",(int)eeprom.temp_alarm_value3);
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
    eeprom.pwm_info = ISP_Read(PWM_ADDR_EEPROM);
    PWMB_CCR7 = ((eeprom.pwm_info) & 0x0F)*184;
    PWMB_CCR8 = (eeprom.pwm_info>>4)*184;

    /*    LED开关状态初始化    */
    eeprom.led_info = ISP_Read(LED_ADDR_EEPROM);
    if( eeprom.led_info & 0X01 )
    {
        DC_24V_out(1);
    }else
    {
        DC_24V_out(0);
    }

    /*    220V输出开关状态初始化    */
    eeprom.ac220_info = ISP_Read(AC220_ADDR_EEPROM);
    if( eeprom.ac220_info & 0X01 )
    {
        INTCLKO |= 0x10;
        temp.temp_scan_allow_flag = 1;
        AC_Out1 = AC_Out2 = AC_Out3 = 0;
    }else
    {
        INTCLKO &= ~0x10;
        temp.temp_scan_allow_flag = 0;
        AC_Out1 = AC_Out2 = AC_Out3 = 1;
    }
    AC_220V_out(eeprom.ac220_info>>1);

    /*    三路NTC alarm value初始化    */
    eeprom.temp_alarm_value1 = ISP_Read(TEMP_ALARM1);
    eeprom.temp_alarm_value2 = ISP_Read(TEMP_ALARM2);
    eeprom.temp_alarm_value3 = ISP_Read(TEMP_ALARM3);

    temp.temp_alarm_value1 = eeprom.temp_alarm_value1;
    temp.temp_alarm_value2 = eeprom.temp_alarm_value2;
    temp.temp_alarm_value3 = eeprom.temp_alarm_value3;
}
