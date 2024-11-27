#ifndef _PID_H_
#define _PID_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//================================================���� PID �����ṹ��===============================================
typedef struct                                                                  //���ȶ���PID�ṹ�����ڴ��һ��PID������
{
    float kp,ki,kd;                                                             //����ϵ��
    float error,lastError;                                                      //���ϴ����
    float integral,maxIntegral;                                                 //���֡������޷�
    float output,maxOutput;                                                     //���������޷�
}PID;
//================================================���� PID �����ṹ��===============================================

//================================================���� Thres ��������================================================
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);        //���ڳ�ʼ��PID�����ĺ���
void PID_Calc(PID *pid,float reference,float feedback);                         //����һ��PID����
//================================================���� Thres ��������================================================
#endif
