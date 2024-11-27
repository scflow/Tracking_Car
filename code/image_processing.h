#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//=====================================���� image_processing��������==========================================
#define Thres                       128     //��ֵ
#define ERROR                       2       //���

#define GrayScale                   256

#define BLACK                       0
#define WHITE                       255

#define IMAGE_H                     120     //ͼ��߶�
#define IMAGE_W                     94      //ͼ����
//=====================================���� image_processing��������==========================================

//=====================================���� image_processingȫ�ֱ���=========================================
extern uint8 Image_deal[MT9V03X_H][MT9V03X_W];
extern uint8 Image_use_cut[IMAGE_H][IMAGE_W];
extern uint8 Image_use[MT9V03X_H][MT9V03X_W];
extern float  BlackThres;                             //�ڰ���ֵ
//=====================================���� image_processingȫ�ֱ���==========================================

//=====================================���� image_processing��������==========================================
void    Iteration_Threshould(void);                                                 //������������ֵ
void    Binarization(void);                                                         //��ֵ��
void    cuttingimage(void);                                                         //ͼ�����
void    image_draw_rectan(void);                                                    //��ͼ�񻯺ڿ�
//=====================================���� image_processing��������==========================================
#endif
