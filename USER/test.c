#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "exti.h"
#include "key.h"
#include "rtc.h"
#include "timer.h"
#include "stdio.h"
#include "usmart.h"
#include "UI.h"

/**********************************************************************************/
/*												�ӿ�																		                */
/*  ����ģ�飺�ⲿ�����ж� PC2����	PA4ת��								 											  */ 
/*	TIM3��500ms��ʱ��																															*/
/*	TIM4��USMART																																	*/
/*	�����ܳ���150cm																																*/
/**********************************************************************************/

extern float V;
extern u16 V_RPM;
extern u16 quanshu_tenfold;		//ʮ����Ȧ��
  int main(void)
 {
	 u8 t=0;	
	 u16 Time=0,dis_pre=0;
	 float V_ave;	//ƽ���ٶ�
	 char code_ave[5],code_v[5],code_vr[5], mileage[5];//��Ϣ��������
	 
	delay_init(72);	    	 			//��ʱ������ʼ��	  
	Stm32_Clock_Init(9);		  //ϵͳʱ������
	uart_init(72,9600);	 	 		  //���ڳ�ʼ��Ϊ9600
	usmart_dev.init(72);			//��ʼ��USMART	
	EXTIX_Init();							//�ⲿ�жϳ�ʼ��
	TIM3_Int_Init(5000,7199); //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	LCD_Init();				 				//  PE.ALL,PD1~5  
	LCD_Display_Dir(1);				//������ʾ����Ϊ������ʾ
	LED_Init();		  					//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();								//��ʼ������

		while(RTC_Init())
		{	//�ȴ�RTC��ʼ��
			delay_ms(200);
			printf("rtc waiting```\r\n");
		} 
		printf("finish\n");
		display_jing();
		//��ʼ�����
   while(1)
	{	
		POINT_COLOR=RED;
		if(t!=calendar.sec)		//ʱ����º���ʾ��ʱ��
		{
				t=calendar.sec;	
				LCD_ShowNum(200,135,calendar.min,2,32);		
				LCD_ShowString(230,135,200,32,32,":");			
				LCD_ShowNum(248,135,calendar.sec,2,32);
			
				Time = 60*calendar.min+calendar.sec;//������ʱ
				if(dis_pre != quanshu_tenfold)			//��λ�Ʊ仯
				{
					V_ave=(0.154*quanshu_tenfold)/Time;
					dis_pre = quanshu_tenfold;
					sprintf(code_ave,"%.2f",V_ave);	
					LCD_ShowString(100,200,200,32,32,code_ave);//�޸�һ��λ��
					
				}
		}
			
		sprintf(code_v,"%.2f",V);					//float ת �ַ���  
		LCD_ShowString(365,275,200,32,32,code_v);		//�ٶ�
		
		sprintf(code_vr,"%d",V_RPM);					//float ת �ַ���  
		LCD_ShowString(365,275,200,32,32,code_vr);		//ת��
 
		sprintf(mileage,"%.2f",0.154*quanshu_tenfold);
		LCD_ShowString(250,225,200,24,32,mileage);	//���
		
	  display_dong(V_RPM,V,50);
		delay_ms(300);
	}
 }
