#include	"config.h"
#include	"gpio.h"
#include	"UART.h"
#include	"delay.h"
#include	"PWM.h"
/*
* 协议格式 66 XX XX XX XX YY ZZ 99
* 帧头 66
* 摇杆数据 XX XX XX XX
* 功能数据 YY
* CRC校验  ZZ
* 帧尾 99
*/
void main(void)
{	
	u16 LostCount = 0;//失联计数
	PWMA_Duty.PWM1_Duty = 128;
	PWMA_Duty.PWM2_Duty = 128;
	PWMA_Duty.PWM3_Duty = 128;
	PWMA_Duty.PWM4_Duty = 128;
	GPIO_config();
	UART_config();
	PWM_config();
	EA = 1;

	while (1)
	{
		delay_ms(1);
		if(COM1.RX_TimeOut > 0)		//超时计数
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(RX1_Buffer[0]==0x66 && RX1_Buffer[7]==0x99) //帧头帧尾判断 不做CRC校验
				{
					//LostCount = 0;
					//PWM输出
					PWMA_Duty.PWM1_Duty = RX1_Buffer[1];
					PWMA_Duty.PWM2_Duty = RX1_Buffer[2];
					PWMA_Duty.PWM3_Duty = RX1_Buffer[3];
					PWMA_Duty.PWM4_Duty = RX1_Buffer[4];
					UpdatePwm(PWMA, &PWMA_Duty);
					//IO输出 未测试
					P34 = (RX1_Buffer[5] >> 0)& 0x01;
					P35 = (RX1_Buffer[5] >> 1)& 0x01;
					P36 = (RX1_Buffer[5] >> 2)& 0x01;
					P37 = (RX1_Buffer[5] >> 3)& 0x01;
					if(COM1.RX_Cnt > 0)
					{
						//for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//收到的数据原样返回 测试打印
					}
				}
				else
				{
					//失联回中 未测试
//					if(++LostCount > 1000)//3*1000ms
//					{
//						LostCount = 0;
//						PWMA_Duty.PWM1_Duty = 128;
//						PWMA_Duty.PWM2_Duty = 128;
//						PWMA_Duty.PWM3_Duty = 128;
//						PWMA_Duty.PWM4_Duty = 128;
//						UpdatePwm(PWMA, &PWMA_Duty);
//					}

				}
				COM1.RX_Cnt = 0;
			}
		}
	}
}



