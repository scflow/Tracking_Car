#ifndef _SEARCH_MIDLINE_H_
#define _SEARCH_MIDLINE_H_

#include "zf_common_typedef.h"
#include "zf_device_type.h"

//=====================================���� search_midline��������==========================================
#define Mid_road  46
//=====================================���� search_midline��������==========================================

//=====================================���� search_midlineȫ�ֱ���==========================================
extern uint8 L_corner_row[5];//��յ�������
extern uint8 L_corner_col[5];//��յ�������
extern uint8 R_corner_row[5];//�ҹյ�������
extern uint8 R_corner_col[5];//�ҹյ�������
extern uint8 Mid_line[MT9V03X_H];
extern uint8 Left_edge[MT9V03X_H];
extern uint8 Right_edge[MT9V03X_H];
extern float error,last_error,YERR;
extern uint8 far,mid,near;
//=====================================���� search_midlineȫ�ֱ���==========================================

//=====================================����search_midline�����ṹ��==========================================

//=====================================����search_midline�����ṹ��==========================================

//=====================================���� search_midline��������==========================================
void    clear_find_point(void);         //������߽��ʼ��
void    search_neighborhood(void);      //�������ұ߽�
int16   calc_diff(int16 x, int16 y);    //��Ⱥ�
float   limit(float x, int32 y);        //�޷�
void    get_turning_point(void);        //�յ���
float   Get_angle(float Ax, float Ay, float Bx, float By, float Cx, float Cy);    //��͸��֪�������γɵĽǶ�
void    midline(void);                  //������
void    get_error(void);                //��ƫ��
void    Horizontal_line(void);
//=====================================���� search_midline��������==========================================

#endif
