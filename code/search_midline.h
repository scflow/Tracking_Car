#ifndef _SEARCH_MIDLINE_H_
#define _SEARCH_MIDLINE_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//=====================================定义 search_midline基本配置==========================================
#define Mid_road  46
//=====================================定义 search_midline基本配置==========================================

//=====================================定义 search_midline全局变量==========================================
extern uint8 L_corner_row[5];//左拐点所在行
extern uint8 L_corner_col[5];//左拐点所在列
extern uint8 R_corner_row[5];//右拐点所在行
extern uint8 R_corner_col[5];//右拐点所在列
extern uint8 Mid_line[MT9V03X_H];
extern uint8 Left_edge[MT9V03X_H];
extern uint8 Right_edge[MT9V03X_H];
extern float error,last_error,YERR;
extern uint8 far,mid,near;
//=====================================定义 search_midline全局变量==========================================

//=====================================定义search_midline参数结构体==========================================

//=====================================定义search_midline参数结构体==========================================

//=====================================声明 search_midline基础函数==========================================
void    clear_find_point(void);         //八邻域边界初始化
void    search_neighborhood(void);      //八邻域找边界
int16   calc_diff(int16 x, int16 y);    //差比和
float   limit(float x, int32 y);        //限幅
void    get_turning_point(void);        //拐点检测
float   Get_angle(float Ax, float Ay, float Bx, float By, float Cx, float Cy);    //逆透视知三点求形成的角度
void    midline(void);                  //找中线
void    get_error(void);                //求偏差
void    Horizontal_line(void);
//=====================================声明 search_midline基础函数==========================================

#endif
