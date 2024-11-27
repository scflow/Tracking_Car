#ifndef _PID_H_
#define _PID_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//================================================定义 PID 参数结构体===============================================
typedef struct                                                                  //首先定义PID结构体用于存放一个PID的数据
{
    float kp,ki,kd;                                                             //三个系数
    float error,lastError;                                                      //误差、上次误差
    float integral,maxIntegral;                                                 //积分、积分限幅
    float output,maxOutput;                                                     //输出、输出限幅
}PID;
//================================================定义 PID 参数结构体===============================================

//================================================声明 Thres 基础函数================================================
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);        //用于初始化PID参数的函数
void PID_Calc(PID *pid,float reference,float feedback);                         //进行一次PID计算
//================================================声明 Thres 基础函数================================================
#endif
