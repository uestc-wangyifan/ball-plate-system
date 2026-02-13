#include "key.h"
#include "stm32f10x.h"
#include "delay.h"

extern unsigned char mode,mode_flag;
extern unsigned long int start_time;

void Key_init(void)
{
    GPIO_InitTypeDef GPIO_initStr;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	
	GPIO_initStr.GPIO_Pin=GPIO_Pin_13;     //key_0
	GPIO_initStr.GPIO_Mode=GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOC,&GPIO_initStr);
	
	GPIO_initStr.GPIO_Pin=GPIO_Pin_14;     //key_1
	GPIO_initStr.GPIO_Mode=GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOC,&GPIO_initStr);

	GPIO_initStr.GPIO_Pin=GPIO_Pin_15;     //key_2
	GPIO_initStr.GPIO_Mode=GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOC,&GPIO_initStr);
	
	
}


void Key_scanf(void)   
{
	 if(key0==0)        //选择
	 {
		mode_flag=1;start_time=0;
	 }
	 else if(key1==0)   //下
	 {
         mode_flag=0;
		mode++;if(mode>3)mode=3;
	 }
	 else if(key2==0)   //上
	 {
         mode_flag=0;
		 mode--;if(mode<1)mode=1;
	 }
}
