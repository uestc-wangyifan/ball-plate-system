#include "usart.h"	
#include "Servo.h"
#include "pid.h"
/////////////////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

///////////////////////////////////////////////////////////////////////////////

int openmv_data[5]; //openmv发来的数据包

extern int X_measure, Y_measure;   //小球测量坐标
extern int motor_flag;             //电机使能标志
extern int X_measure, Y_measure;   //小球测量坐标
extern float X_calcu,Y_calcu;      //小球理论坐标
extern int X_middle,Y_middle;      //平板中间坐标
extern int ServoX_PWM,ServoY_PWM;  //两个舵机的PWM值
extern int err_time;               //检测不到小球的异常时间


void USART1_init(int bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); 		//初始化串口1
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口1接收中断
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART1, ENABLE);//使能串口1 
}


/*************************************************************

		 串口1接收openmv数据的协议在此服务程序中，
          必须严格保证数据接受和 PID 处理的同步

注：openmv端若检测到球，则通过串口发送中断和坐标，否则不发送

*************************************************************/
void USART1_IRQHandler(void)
{
	static int i=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
        err_time=0; //异常时间清零
        
		openmv_data[i++] = USART_ReceiveData(USART1);	//接收数据
		if(openmv_data[0]!=0xa3) i=0;             		//判断第一个帧头
	    if((i==2)&&(openmv_data[1]!=0xb3)) i=0;    		//判断第二个帧头	
		if(i==5)                           			      //代表一组数据传输完毕
		{
			i = 0;			
			if( Data_test(openmv_data) )             //判断数据合理性,更新x,y测量值
			{
				X_measure = openmv_data[2]; //读取x坐标
				Y_measure = openmv_data[3]; //读取y坐标
                
            /****************************************************
                 此时得到了一组可以信任的数据，同步处理PID
            *****************************************************/
                
//                X_calcu = X_middle;
//                Y_calcu = Y_middle;    //取理论值为中值（期望小球在板子中间）
                
                ServoX_PWM = X_PD_pos_value(X_measure,X_calcu);
                ServoY_PWM = Y_PD_pos_value(Y_measure,Y_calcu);
                
                if(motor_flag)   //处理并装载PWM
                {
                    ServoX_PWM=1400+ServoX_PWM;
                    ServoY_PWM=1400+ServoY_PWM;
                    PWM_Xianfu(880,2000,850,2000,&ServoX_PWM,&ServoY_PWM);
                    
                    Servo_Control(ServoX_PWM,ServoY_PWM);
                }
		    }
		}
	}
}

int Data_test(int data[])
{
	if(data[0]!=0xa3) return 0;  //帧头
	if(data[1]!=0xb3) return 0;  //帧头
	if(data[4]!=0xc3) return 0;  //帧尾
	
	if(data[2]>150)   return 0;  //x坐标上限
	if(data[3]>120)   return 0;  //y坐标上限
	
	return 1;
}



