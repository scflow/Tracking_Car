#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//=====================================定义 image_processing基本配置==========================================
#define Thres                       128     //阈值
#define ERROR                       2       //误差

#define GrayScale                   256

#define BLACK                       0
#define WHITE                       255

#define IMAGE_H                     120     //图像高度
#define IMAGE_W                     94      //图像宽度
//=====================================定义 image_processing基本配置==========================================

//=====================================定义 image_processing全局变量=========================================
extern uint8 Image_deal[MT9V03X_H][MT9V03X_W];
extern uint8 Image_use_cut[IMAGE_H][IMAGE_W];
extern uint8 Image_use[MT9V03X_H][MT9V03X_W];
extern float  BlackThres;                             //黑白阈值
//=====================================定义 image_processing全局变量==========================================

//=====================================声明 image_processing基础函数==========================================
void    Iteration_Threshould(void);                                                 //迭代法计算阈值
void    Binarization(void);                                                         //二值化
void    cuttingimage(void);                                                         //图像裁切
void    image_draw_rectan(void);                                                    //给图像化黑框
//=====================================声明 image_processing基础函数==========================================
#endif
