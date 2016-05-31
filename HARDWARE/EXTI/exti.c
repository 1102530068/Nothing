#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h" 

u16 signal=0;
//�ⲿ�ж�9~5�������
void EXTI9_5_IRQHandler(void)
{			

	signal++;
 	EXTI->PR=1<<5;     //���LINE5�ϵ��жϱ�־λ  
}
void EXTI15_10_IRQHandler(void)
{			
		RTC_Set(2016,5,20,0,0,0);		//ʱ������
		EXTI->PR=1<<15; 						//���LINE15���жϱ�־λ
}
//��ʼ��PA0,PC5,PA15Ϊ�ж�����.
void EXTI_Init(void)
{
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	JTAG_Set(SWD_ENABLE);		//�ر�JTAG,����SWD
	GPIOC->CRL&=0XFF0FFFFF;	//PC5���ó�����	  
	GPIOC->CRL|=0X00800000;   
	GPIOC->ODR|=1<<5;	   		//PC5����  
	GPIOC->ODR|=1<<5;	   		//PC5���� 
	//Ex_NVIC_Config(GPIO_A,0,RTIR); 		//�����ش���
	Ex_NVIC_Config(GPIO_C,5,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_A,15,FTIR);		//�½��ش���

//	MY_NVIC_Init(2,2,EXTI0_IRQn,2);    	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
	MY_NVIC_Init(2,0,EXTI15_10_IRQn,2);	//��ռ2�������ȼ�0����2	   
}
