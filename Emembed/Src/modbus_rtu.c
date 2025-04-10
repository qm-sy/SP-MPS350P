#include "modbus_rtu.h"

MODBIS_INFO modbus;

/**
 * @brief	modbus_rtu  无奇偶校验
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Event( void )
{
    uint16_t crc,rccrc;
    
    /*1.接收完毕                                           */
    if( rs485.RX2_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485.RX2_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.RX2_buf, rs485.RX2_rev_cnt-2);
        rccrc = (rs485.RX2_buf[rs485.RX2_rev_cnt-2]) | (rs485.RX2_buf[rs485.RX2_rev_cnt-1]<<8);

        /*4.清空接收计数                                    */
        rs485.RX2_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485.RX2_buf[0] == MY_ADDR )
            {
                switch ( rs485.RX2_buf[1] )
                {
                    case 0x03:
                        Modbus_Fun3();
                        break;

                    case 0x06:
                        Modbus_Fun6();
                        break;  

                    case 0x10:  
                        Modbus_Fun16();

                    default:
                        break;
                }
            }
        }
    }
}

/**
 * @brief	读输出寄存器  03
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun3( void )
{
    uint16_t i;

    modbus.send_value_addr  = 3;                 //DATA1 H 位置
    modbus.byte_cnt   = (rs485.RX2_buf[4]<<8 | rs485.RX2_buf[5]) *2;
    modbus.start_addr = rs485.RX2_buf[2]<<8 | rs485.RX2_buf[3];

    rs485.TX2_buf[0]  = MY_ADDR;                //Addr
    rs485.TX2_buf[1]  = 0x03;                   //Fun
    rs485.TX2_buf[2]  = modbus.byte_cnt;        //Byte Count
    
    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {
            /*  0x00                  */
            case 0:
                modbus.byte_info_H  = 0X00;
                modbus.byte_info_L  = 0X00;   

                break;

            /*  0x01  NTC1、NTC2温度查询                           */
            case 1:
                modbus.byte_info_L = temp.temp_alarm_value1;
                modbus.byte_info_H = temp.temp_alarm_value2;   

                break;

            /*  0x02  NTC3温度查询                */
            case 2:
                modbus.byte_info_H = 0X00;   
                modbus.byte_info_L = temp.temp_alarm_value3;

                break;

            /*  0x03 环境温湿度查询                   */
            case 3:
                modbus.byte_info_H = 0x41;           
                modbus.byte_info_L = 0x19;          
                // modbus.byte_info_L = get_temp(NTC_1);
                // modbus.byte_info_H = get_temp(NTC_3);  
                break;

            /*  0x04 LED、三路加热使能状态                           */
            case 4:
                if( ac_220.ac220_out1_flag == 1 )
                {
                    modbus.byte_info_L |= 0x01;
                }else
                {
                    modbus.byte_info_L &= 0xFE;
                }

                if( ac_220.ac220_out2_flag == 1 )
                {
                    modbus.byte_info_L |= 0x02;
                }else
                {
                    modbus.byte_info_L &= 0xFD;
                }

                if( ac_220.ac220_out3_flag == 1 )
                {
                    modbus.byte_info_L |= 0x04;
                }else
                {
                    modbus.byte_info_L &= 0xFB;
                }

                modbus.byte_info_H = 0X00;   

                break;

            /*  0x05 风速、led状态查询                          */
            case 5:
                modbus.byte_info_L = ~DC_24V;       //LED状态 bit4
                modbus.byte_info_L = ((PWMB_CCR7 / 184) | (modbus.byte_info_L<<3));         //风速 bit0~3
                modbus.byte_info_H = 0X00;   

                break;
            default:
                break;
        }
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_H;
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_L;
    }
    slave_to_master(3 + modbus.byte_cnt);
}


/**
 * @brief	写单个输出寄存器  06
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun6( void )
{
    switch (rs485.RX2_buf[3])
    {
        /*  0x10  两路PWM 风速设置                 */
        case 0x10:             
            memcpy(rs485.TX2_buf,rs485.RX2_buf,8);                            
            
            PWMB_CCR7 = PWMB_CCR8 = rs485.TX2_buf[5] * 184;

            rs485.TX2_send_bytelength = 8;

            DR2 = 1;                                    //485可以发送
            delay_ms(5);
            S2CON |= S2TI;                              //开始发送

            eeprom.pwm_info = rs485.TX2_buf[5];
            eeprom_data_record();

          break;

        /*  0x11  LED 及 220V输出 开关状态设置                          */
        case 0x11:                                         
            memcpy(rs485.TX2_buf,rs485.RX2_buf,8);

            if( rs485.TX2_buf[5] & 0X01 )
            {
                DC_24V_out(1);
            }else
            {
                DC_24V_out(0);
            }

            ac_220.ac220_out1_flag = ((modbus.byte_info_L >> 1) & 0x01);
            ac_220.ac220_out2_flag = ((modbus.byte_info_L >> 2) & 0x01);
            ac_220.ac220_out3_flag = ((modbus.byte_info_L >> 3) & 0x01);

            rs485.TX2_send_bytelength = 8;

            DR2 = 1;                                    //485可以发送
            delay_ms(5);
            S2CON |= S2TI;                              //开始发送

            eeprom.led_info = rs485.TX2_buf[5];
            eeprom_data_record();

            break;

        /*  0x12  NTC1 NTC2 alarm value 设置                          */
        case 0x12:                                         
            memcpy(rs485.TX2_buf,rs485.RX2_buf,8);

            temp.temp_alarm_value1 = rs485.TX2_buf[5];
            temp.temp_alarm_value2 = rs485.TX2_buf[4];
            
            rs485.TX2_send_bytelength = 8;
            DR2 = 1;                                    //485可以发送
            delay_ms(5);
            S2CON |= S2TI;                              //开始发送

            eeprom.temp_alarm_value1 = temp.temp_alarm_value1;
            eeprom.temp_alarm_value2 = temp.temp_alarm_value2;
            eeprom_data_record();

            break;  
            
        /*  0x13  NTC3 alarm value 设置                   */
        case 0x13:                                         
            memcpy(rs485.TX2_buf,rs485.RX2_buf,8);

            temp.temp_alarm_value3 = rs485.TX2_buf[5];
            
            rs485.TX2_send_bytelength = 8;
            DR2 = 1;                                    //485可以发送
            delay_ms(5);
            S2CON |= S2TI;                              //开始发送

            eeprom.temp_alarm_value3 = temp.temp_alarm_value3;
            eeprom_data_record();

            break;
        default:
            break;   
    }
}

/**
 * @brief	写多个输出寄存器  16
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun16( void )
{
    uint16_t crc;
    uint16_t i;

    modbus.rcv_value_addr = 7;                  //DATA1 H位置
    modbus.byte_cnt   = rs485.RX2_buf[6];
    modbus.start_addr = rs485.RX2_buf[2]<<8 | rs485.RX2_buf[3];

    memcpy(rs485.TX2_buf,rs485.RX2_buf,6);
    crc = MODBUS_CRC16(rs485.TX2_buf,6);
    rs485.TX2_buf[7] = crc>>8;                 //CRC H
    rs485.TX2_buf[6] = crc;                    //CRC L

    rs485.TX2_send_bytelength = 8;
    DR2 = 1;                                   //485可以发送
    delay_ms(2);
    S2CON |= S2TI;  
    delay_ms(1);
    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++)
    {
        modbus.byte_info_H = rs485.RX2_buf[modbus.rcv_value_addr];
        modbus.byte_info_L = rs485.RX2_buf[modbus.rcv_value_addr + 1];
        switch (i)
        {
            /*  0x10  两路PWM 开关状态及风速设置                 */
            case 0X10:
                PWMB_CCR7 = PWMB_CCR8 = modbus.byte_info_L * 184;

                eeprom.pwm_info = modbus.byte_info_L;
                break;
            
            /*  0X11  24V LED  AC220V 1\2\3开关状态设置                          */
            case 0X11:
                if( modbus.byte_info_L & 0X01 )
                {
                    DC_24V_out(1);
                }else
                {
                    DC_24V_out(0);
                }
                ac_220.ac220_out1_flag = ((modbus.byte_info_L >> 1) & 0x01);
                ac_220.ac220_out2_flag = ((modbus.byte_info_L >> 2) & 0x01);
                ac_220.ac220_out3_flag = ((modbus.byte_info_L >> 3) & 0x01);

                eeprom.led_info = modbus.byte_info_L;
                break;

            /*  0X12  NTC1 NTC2 alarm value 设置                   */
            case 0X12:
                temp.temp_alarm_value1 = modbus.byte_info_L;
                temp.temp_alarm_value2 = modbus.byte_info_H;
                

                eeprom.temp_alarm_value1 = temp.temp_alarm_value1;
                eeprom.temp_alarm_value2 = temp.temp_alarm_value2;
                break;

            
            /*  0X13  NTC3 alarm value 设置                        */
            case 0X13:
                temp.temp_alarm_value3 = modbus.byte_info_L;

                eeprom.temp_alarm_value3 = temp.temp_alarm_value3;
                break;

            default:
                break;
        }
        modbus.rcv_value_addr += 2;         //从Value1_H →→ 从Value2_H
    }
    


    //DR2 = 1;                                   //485可以发送
    //delay_ms(5);
    

    eeprom_data_record();                      //记录更改后的值
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}

/**
 * @brief	从机回复主机
 *  
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
void slave_to_master(uint8_t length)
{
    uint16_t crc;

    crc = MODBUS_CRC16(rs485.TX2_buf,length);

    rs485.TX2_buf[length] = crc;                 //CRC H
    rs485.TX2_buf[length+1] = crc>>8;                  //CRC L

    rs485.TX2_send_bytelength = length + 2;

    DR2 = 1;                                        //485可以发送
    delay_ms(2);
    S2CON |= S2TI;                                  //开始发送
}
