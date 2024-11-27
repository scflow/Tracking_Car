#include "zf_device_mt9v03x.h"
#include "image_processing.h"

int16  EdgeThres = 18;                              //晚上20  白天25 18
float  BlackThres;                                  //黑白阈值

uint8 temp = 0;
uint8 Image_deal[MT9V03X_H][MT9V03X_W];

uint8 Image_use_cut[IMAGE_H][IMAGE_W];
uint8 Image_use[MT9V03X_H][MT9V03X_W];
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     迭代法计算阈值
// 参数说明     void
// 返回参数     void
// 使用示例     Iteration_Threshould();
//-------------------------------------------------------------------------------------------------------------------
void Iteration_Threshould()
{
    uint16_t i = 0,j = 0,N0 = 0,N1 = 0,flag = 0;    //N0,N1计数大于和小于阈值的像素点数
    float T0,T1,T2,T_center;                        //T0,T1分别大于和小于的均值
    uint32_t S0 = 0,S1 = 0;                         //S0,S1分别累加大于和小于阈值的和
    T2 = BlackThres;
    do{
        for(i=0;i<MT9V03X_H;i++){
            for(j=0;j<MT9V03X_W;j++)
            {
                if(mt9v03x_image[i][j] < T2)
                {
                    S0 += mt9v03x_image[i][j];
                    N0++;
                }
                else
                {
                    S1 += mt9v03x_image[i][j];
                    N1++;
                }
            }
    }
    T0 = (float)S0/N0;
    T1 = (float)S1/N1;
    T_center = (T0+T1)/2;
    if(T2 < T_center)
    {
        if((T_center - T2)> ERROR){
        flag = 1;
    }
    else    flag = 0;
    }
    else
    {
      if((T2 - T_center) > ERROR)   flag = 1;
      else  flag = 0;
    }
    T2 =T_center;
    BlackThres = T2;
  }
  while(flag);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     二值化
// 参数说明     void
// 返回参数     void
// 使用示例     Binarization();
//-------------------------------------------------------------------------------------------------------------------
void Binarization()
{
  for(int i=0;i< MT9V03X_H;i++)
  {
    for(int j=0;j<MT9V03X_W;j++)
    {
      if(mt9v03x_image[i][j]>BlackThres)
      {
        Image_deal[i][j]=255;
      }
      else
      {
        Image_deal[i][j]=0;
      }
      Image_use[i][j] = Image_deal[i][j];
    }
  }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     图像裁切
// 参数说明     void
// 返回参数     void
// 使用示例     void cuttingimage();
//-------------------------------------------------------------------------------------------------------------------
void cuttingimage(void)
{
    int i, j;
    for (i = 0; i < IMAGE_H; i++)
    {
        for (j = 0; j < IMAGE_W; j++)
        {
            Image_use_cut[i][j] = Image_deal[i][j+47];
            Image_use[i][j] = Image_use_cut[i][j];
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介    给图像画黑框
//  参数说明    void
//  返回参数    void
//  使用示例    image_draw_rectan();
//-------------------------------------------------------------------------------------------------------------------
void image_draw_rectan(void)
{
    uint8 i = 0;
    for (i = 0; i < MT9V03X_H; i++)
    {
        Image_use[i][0] = 0;
        Image_use[i][1] = 0;
        Image_use[i][MT9V03X_W - 1] = 0;
        Image_use[i][MT9V03X_W - 2] = 0;
    }
    for (i = 0; i < MT9V03X_W; i++)
    {
        Image_use[0][i] = 0;
        Image_use[1][i] = 0;
    }
}

