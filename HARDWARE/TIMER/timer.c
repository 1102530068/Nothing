#include "timer.h"
#include "led.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
float V=0;
u16 V_RPM=0;
 u16 signal_sum=0;
 u16 quanshu_tenfold=0;		//圈数的十倍
 extern u16 signal;
 extern u16 signal_RPM;
 
//定时器3中断服务程序	 0.5s进一次  计算车速和转速
void TIM3_IRQHandler(void)
{ 		 
	if(TIM3->SR&0X0001)//溢出中断
	{		
		V=signal*0.9;					//速度转换 Km/h   和实际相差一倍 需要/2
		V_RPM=signal_RPM*15;	//转速转换 r/min		需要*2   //W T F
		signal_sum+=signal;
		if(signal_sum>=120)			//刹车盘一圈12个信号
		{
			quanshu_tenfold++;
			signal_sum %= 120;	//留下不足120的部分
		}
		printf("%d    V=%f    V_RPM=%d \n",signal,V,V_RPM);
		signal=0;
		signal_RPM=0;
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器设置
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}


