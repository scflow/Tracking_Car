#include "pid.h"

//首先定义PID结构体用于存放一个PID的数据
//typedef struct
//{
//   	float kp,ki,kd;//三个系数
//    float error,lastError;//误差、上次误差
//    float integral,maxIntegral;//积分、积分限幅
//    float output,maxOutput;//输出、输出限幅
//}PID;


//-------------------------------------------------------------------------------------------------------------------
//  函数简介      用于初始化pid参数的函数
//  参数说明      PID *pid PID结构体
//  参数说明      浮点型 比例 P
//  参数说明      浮点型 积分 I
//  参数说明      浮点型 微分 D
//  参数说明      float maxI 浮点型 积分限幅
//  参数说明      float maxOut 浮点型 输出限幅
//  返回参数      void
//  使用示例      PID_Init();
//-------------------------------------------------------------------------------------------------------------------
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;

}
 
//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      pid计算
//  参数说明      PID *pid PID结构体
//  参数说明      reference 浮点型 目标值
//  参数说明      feedback 浮点型 反馈值
//  返回参数      void
//  使用示例      PID_Calc(PID *pid,float reference,float feedback);
//  备注信息      计算结果放在pid结构体的output成员中
//-------------------------------------------------------------------------------------------------------------------
void PID_Calc(PID *pid,float reference,float feedback)
{
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
    //计算微分
    float dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例
    float pout=pid->error*pid->kp;
    //计算积分
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}
