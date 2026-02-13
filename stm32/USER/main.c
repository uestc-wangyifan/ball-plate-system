#include "delay.h"
#include "usart.h"
#include "Servo.h"
#include "timer.h"
#include "oled.h"
#include "pid.h"
#include "key.h"

int X_measure, Y_measure;     //小球测量坐标
float X_calcu,Y_calcu;        //小球理论坐标
int X_middle=80,Y_middle=60 ; //平板中间坐标
int err_time=0;               //检测不到小球的异常时间

int motor_flag=0;             //舵机使能标志
int ServoX_PWM,ServoY_PWM;    //两个舵机的PWM值

unsigned char mode=1,mode_flag=0;
unsigned char one_logo=1;

unsigned long int start_time;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    OLED_Init();
    Key_init();
    Servo_Init(20000-1,72-1);
    Servo_Control(1400,1400);  //先给一个让板子水平时的占空比
    USART1_init(115200);
    TIM2_Init(10000-1,72-1);
    
    OLED_ShowStr(0,0,"x:",2);OLED_ShowStr(0,2,"y:",2);OLED_ShowStr(60,5,"Mode:",2);
    
    motor_flag=1;
    
    while(1)
    {
        Key_scanf(); //比较草率的按键扫描，按一下容易多次触发，但耐不住它简单呀，凑合用，核心不在这
        OLED_ShowNum(20,0,X_measure,2);OLED_ShowNum(20,2,Y_measure,2); //显示小球测量坐标
        OLED_ShowNum(5,5,start_time/100,2);  //读秒显示
        
        switch(mode)
        {
            case 1:OLED_ShowNum1(75,5,mode,2);break;
            case 2:OLED_ShowNum1(75,5,mode,2);break;
            case 3:OLED_ShowNum1(75,5,mode,2);break;
        }
        if(mode_flag)
            OLED_ShowStr(120,5,"*",2);  //我定义为确认键，显示*的时候代表已经进入了这个模式
        else
            OLED_ShowStr(120,5,"   ",2);
        
        err_time++;      //异常时间累加
        if(err_time>30)  //小球可能已经脱板
        {
            Servo_Control(1400,1400); //回到水平位置
        }
        delay_ms(10);   //适当增加这个数值，可以给按键消抖
    }
}



void TIM2_IRQHandler(void)  //有关时间的操作放在中断里就行了，下面写的比较潦草
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        start_time++;   //每10ms++
        
        if(mode_flag)
        {
            switch(mode)
            {
                case 1:
                {
                    X_calcu=80;Y_calcu=29;
                }
                break;
                case 2:
                {
//                    if(0<start_time&&start_time<300)
//                    {
//                        X_calcu=45;Y_calcu=25;
//                    }
//                    else if(300<start_time)
//                    {
//                        X_calcu=81;Y_calcu=59;
//                    }
                    if(0<start_time&&start_time<300)
                    {
                        X_calcu=45;Y_calcu=25;
                    }
                    else if(300<start_time)
                    {
                        X_calcu=110;Y_calcu=93;
                    }
                }
                break;
                case 3:
                {
                    if(0<start_time&&start_time<400)
                    {
                        X_calcu=45;Y_calcu=25;
                    }
                    else if(300<start_time&&start_time<900)
                    {
                        X_calcu=47;Y_calcu=59;
                    }
                    else if(900<start_time)
                    {
                        X_calcu=81;Y_calcu=59;
                    }
                }
                break;
            }
        }
       
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

