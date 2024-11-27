#include "pid.h"

//���ȶ���PID�ṹ�����ڴ��һ��PID������
//typedef struct
//{
//   	float kp,ki,kd;//����ϵ��
//    float error,lastError;//���ϴ����
//    float integral,maxIntegral;//���֡������޷�
//    float output,maxOutput;//���������޷�
//}PID;


//-------------------------------------------------------------------------------------------------------------------
//  �������      ���ڳ�ʼ��pid�����ĺ���
//  ����˵��      PID *pid PID�ṹ��
//  ����˵��      ������ ���� P
//  ����˵��      ������ ���� I
//  ����˵��      ������ ΢�� D
//  ����˵��      float maxI ������ �����޷�
//  ����˵��      float maxOut ������ ����޷�
//  ���ز���      void
//  ʹ��ʾ��      PID_Init();
//-------------------------------------------------------------------------------------------------------------------
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;

}
 
//����һ��pid����
//����Ϊ(pid�ṹ��,Ŀ��ֵ,����ֵ)������������pid�ṹ���output��Ա��
//-------------------------------------------------------------------------------------------------------------------
//  �������      pid����
//  ����˵��      PID *pid PID�ṹ��
//  ����˵��      reference ������ Ŀ��ֵ
//  ����˵��      feedback ������ ����ֵ
//  ���ز���      void
//  ʹ��ʾ��      PID_Calc(PID *pid,float reference,float feedback);
//  ��ע��Ϣ      ����������pid�ṹ���output��Ա��
//-------------------------------------------------------------------------------------------------------------------
void PID_Calc(PID *pid,float reference,float feedback)
{
 	//��������
    pid->lastError=pid->error;//����error������
    pid->error=reference-feedback;//������error
    //����΢��
    float dout=(pid->error-pid->lastError)*pid->kd;
    //�������
    float pout=pid->error*pid->kp;
    //�������
    pid->integral+=pid->error*pid->ki;
    //�����޷�
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //�������
    pid->output=pout+dout+pid->integral;
    //����޷�
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}
