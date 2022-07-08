#include	"config.h"
#include	"gpio.h"
#include	"UART.h"
#include	"delay.h"
#include	"PWM.h"
/*
* Э���ʽ 66 XX XX XX XX YY ZZ 99
* ֡ͷ 66
* ҡ������ XX XX XX XX
* �������� YY
* CRCУ��  ZZ
* ֡β 99
*/
void main(void)
{	
	u16 LostCount = 0;//ʧ������
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
		if(COM1.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(RX1_Buffer[0]==0x66 && RX1_Buffer[7]==0x99) //֡ͷ֡β�ж� ����CRCУ��
				{
					//LostCount = 0;
					//PWM���
					PWMA_Duty.PWM1_Duty = RX1_Buffer[1];
					PWMA_Duty.PWM2_Duty = RX1_Buffer[2];
					PWMA_Duty.PWM3_Duty = RX1_Buffer[3];
					PWMA_Duty.PWM4_Duty = RX1_Buffer[4];
					UpdatePwm(PWMA, &PWMA_Duty);
					//IO��� δ����
					P34 = (RX1_Buffer[5] >> 0)& 0x01;
					P35 = (RX1_Buffer[5] >> 1)& 0x01;
					P36 = (RX1_Buffer[5] >> 2)& 0x01;
					P37 = (RX1_Buffer[5] >> 3)& 0x01;
					if(COM1.RX_Cnt > 0)
					{
						//for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//�յ�������ԭ������ ���Դ�ӡ
					}
				}
				else
				{
					//ʧ������ δ����
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



