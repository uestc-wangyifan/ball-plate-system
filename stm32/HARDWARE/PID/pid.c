#include "pid.h"


int X_err,X_last_err; 
int Y_err,Y_last_err;  //此次误差和上次误差
int X_err_difference;  
int Y_err_difference;  //误差的差值
int X_err_sum=0;
int Y_err_sum=0;       //误差累加

//float Kp=22,Kd=590,Ki=0.4;

float Kp=20,Kd=550,Ki=0;




int X_PD_pos_value(int measure,int calcu)
{
	X_err = calcu - measure;               //误差
	X_err_difference = X_err - X_last_err; //误差的差值
	X_last_err = X_err;                    //此次误差记录为“上次误差”
	X_err_sum += X_err;                    //误差累加
	I_xianfu(100);
	
	return Kp*X_err + Kd*X_err_difference + Ki*X_err_sum; 
}

int Y_PD_pos_value(int measure,int calcu)
{
	Y_err = calcu - measure;               //误差
	Y_err_difference = Y_err - Y_last_err; //误差的差值
	Y_last_err = Y_err;                    //此次误差记录为“上次误差”
	Y_err_sum += Y_err;                    //误差累加
	I_xianfu(100);
	
	return Kp*Y_err + Kd*Y_err_difference + Ki*Y_err_sum; 
}


void I_xianfu(int max)
{
    if(X_err_sum >max) X_err_sum=max;
	else if(X_err_sum <-max) X_err_sum=-max;
	
	if(Y_err_sum >max) Y_err_sum=max;
	else if(Y_err_sum <-max) Y_err_sum=-max;
}










///**
//  * @brief  位置PID算法实现
//  * @param  目标值，实际值
//  * @retval 通过PID计算后的输出
//  */
//float positionX_PID_value(float target_x,float actual_x)
//{
//    float err_x,err_sum_x,err_difference_x,last_err_x;
//    
//    /*计算目标值与实际值的误差*/
//    err_x=target_x-actual_x;
//    
////     /* 限定闭环死区 *///直接实操看那个圈对应数值大小
////    if((err_x>=-0.5f)&&(err_x<=0.5f))
////    {
////        err_x=0.0f;
////        err_sum_x=0.0f;
////    }
//    
//    /* 积分分离，偏差较大时去掉积分作用 */ 
//    if((err_x>=-5)&&(err_x<=5))
//    {
//        err_sum_x+=err_x;
//        
//        /* 限定积分范围，防止积分饱和 */
//        if (err_sum_x >50)err_sum_x=50;
//        else if (err_sum_x<-50)err_sum_x=-50;
//    }
//    
//    /*两次误差之差*/
//    err_difference_x=err_x-last_err_x;
//    
//    /*记录上次误差*/
//    last_err_x=err_x;
//    
//    return positionX_Kp*err_x+positionX_Ki*err_sum_x+positionX_Kd*err_difference_x;
//}

//float positionY_PID_value(float target_y,float actual_y)
//{
//    float err_y,err_sum_y,err_difference_y,last_err_y;
//    
//    err_y=target_y-actual_y;
////    if((err_y>=-0.5f)&&(err_y<=0.5f))
////    {
////        err_y=0.0f;
////        err_sum_y=0.0f;
////    }
//    if((err_y>=-5)&&(err_y<=5))
//    {
//        err_sum_y+=err_y;
//        if (err_sum_y >50)err_sum_y=50;
//        else if (err_sum_y<-50)err_sum_y=-50;
//    }
//    err_difference_y=err_y-last_err_y;
//    last_err_y=err_y;
//    
//    return positionY_Kp*err_y+positionY_Ki*err_sum_y+positionY_Kd*err_difference_y;
//}

///**
//  * @brief  速度PID算法实现
//  * @param  目标值(位置环的输出)，实际值
//  * @retval 通过PID计算后的输出
//  */
//float speedX_PID_value(float target_x,float actual_x)
//{
//    float err_x,err_difference_x,last_err_x;
//    
//    /*计算目标值与实际值的误差*/
//    err_x=target_x-actual_x;
//    
//    //这下面的比较值设的越大，那么就表示速度环控制精度越低。  
//	//加入把数值为1的偏差都忽略，那么1s的时间小球位移个20个单位坐标都会当作没误差
////    if((err_x<0.1f ) && (err_x>-0.1f))   //这个数值的设置也要弄懂，要设置合理  
////        err_x = 0.0f;   
//    
//    /*两次误差之差*/
//    err_difference_x=err_x-last_err_x;
//    
//    /*记录上次误差*/
//    last_err_x=err_x;
//    
//    return speedX_Kp*err_x+speedX_Kd*err_difference_x;
//}

//float speedY_PID_value(float target_y,float actual_y)
//{
//    float err_y,err_difference_y,last_err_y;
//    
//    /*计算目标值与实际值的误差*/
//    err_y=target_y-actual_y;
//    
//    //这下面的比较值设的越大，那么就表示速度环控制精度越低。  
//	//加入把数值为1的偏差都忽略，那么1s的时间小球位移个20个单位坐标都会当作没误差
////    if((err_y<0.2f ) && (err_y>-0.2f))   //这个数值的设置也要弄懂，要设置合理  
////        err_y = 0.0f;
//    
//    /*两次误差之差*/
//    err_difference_y=err_y-last_err_y;
//    
//    /*记录上次误差*/
//    last_err_y=err_y;
//    
//    return speedY_Kp*err_y+speedY_Kd*err_difference_y;
//}


