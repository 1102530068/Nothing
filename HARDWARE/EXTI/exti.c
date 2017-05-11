#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h" 


u16 signal=0;
u16 signal_RPM=0;
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//����
	if(WK_UP==1)	//WK_UP����PA0
	{				 
		//BEEP=!BEEP;	
		RTC_Set(2016,5,20,0,0,0);		//ʱ�临λ 
	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
} 
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	signal++;
	EXTI->PR=1<<2;  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������  ������ת�ٲ��� 
//  һȦ�˸��ź�
void EXTI4_IRQHandler(void)
{
	signal_RPM++;
	EXTI->PR=1<<4;  //���LINE4�ϵ��жϱ�־λ  
}		   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0/PE3/PE4Ϊ�ж�����.
void EXTIX_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_A,0,RTIR); 	//�����ش��� 
	Ex_NVIC_Config(GPIO_C,2,FTIR); 	//�½��ش���
 	Ex_NVIC_Config(GPIO_A,4,FTIR); 	//�½��ش���
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//��ռ2�������ȼ�3����2  
	MY_NVIC_Init(2,1,EXTI2_IRQn,2);	//��ռ2�������ȼ�1����2	   
	MY_NVIC_Init(2,0,EXTI4_IRQn,2);	//��ռ2�������ȼ�0����2	   
}




///////////////////////////////////////
//�ⲿ�ж�9~5�������
//void EXTI9_5_IRQHandler(void)
//{			

//	signal++;						//�������ź�
// 	EXTI->PR=1<<5;     //���LINE5�ϵ��жϱ�־λ  
//}
//void EXTI15_10_IRQHandler(void)
//{			
//	 
//		RTC_Set(2016,5,20,0,0,0);		//ʱ�临λ  
//		EXTI->PR=1<<15; 						//���LINE15���жϱ�־λ
//}
////��ʼ��PA0,PC5,PA15Ϊ�ж�����.
//void EXTI_Init(void)
//{
//	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
//	JTAG_Set(SWD_ENABLE);		//�ر�JTAG,����SWD
//	GPIOC->CRL&=0XFF0FFFFF;	//PC5���ó�����	  
//	GPIOC->CRL|=0X00800000;   
//	GPIOC->ODR|=1<<5;	   		//PC5����  
//	GPIOC->ODR|=1<<5;	   		//PC5���� 
//	//Ex_NVIC_Config(GPIO_A,0,RTIR); 		//�����ش���
//	Ex_NVIC_Config(GPIO_C,5,FTIR);		//�½��ش���
//	Ex_NVIC_Config(GPIO_A,15,FTIR);		//�½��ش���

////	MY_NVIC_Init(2,2,EXTI0_IRQn,2);    	//��ռ2�������ȼ�2����2
//	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
//	MY_NVIC_Init(2,0,EXTI15_10_IRQn,2);	//��ռ2�������ȼ�0����2	   
//}

