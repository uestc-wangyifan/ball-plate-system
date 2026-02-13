#include "Servo.h"

void Servo_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//初始化定时器
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;//设置待装入捕获比较寄存器的脉冲值
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //高级定时器专属--MOE主输出使能
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //ENABLE-OC1预装载寄存器使能
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH4预装载使能,使能后改变TIM_Pulse(即PWM)的值立刻生效,不使能则下个周期生效
    TIM_ARRPreloadConfig(TIM1, ENABLE); //TIM1在ARR上预装载寄存器使能
    
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Enable);
    
    TIM_Cmd(TIM1, ENABLE);
}

void Servo_Control(u16 X_Angle,u16 Y_Angle)
{
    TIM_SetCompare1(TIM1, Y_Angle);
    TIM_SetCompare4(TIM1, X_Angle);
}


void PWM_Xianfu(int X_min,int X_max,int Y_min,int Y_max,int *MOTORX,int *MOTORY)
{
	if(*MOTORX>X_max) *MOTORX=X_max;
	if(*MOTORX<X_min) *MOTORX=X_min;
	
	if(*MOTORY>Y_max) *MOTORY=Y_max;
	if(*MOTORY<Y_min) *MOTORY=Y_min;
}




