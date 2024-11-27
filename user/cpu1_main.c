/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��


// **************************** �������� ****************************
int16  err;
int16  lasterr;
extern float valtage_now;
void core1_main(void)
{
    system_start();                         // ������ʱ������¼�µ�ǰ��ʱ��
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    //uint8 neighborhood[MT9V03X_H][MT9V03X_W];
    uint16 image_count = 0;
    uint16 Thres_count = 0;
    err = 180;
    ips114_init();
    ips114_show_string(0, 0, "mt9v03x init.");
    mt9v03x_init();
    while(1)
    {
        if(mt9v03x_init())
            ips114_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ����ʱ�������Ʊ�ʾ�쳣
    }
    ips114_clear();
    BlackThres = 130;
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        last_error = error;
        //ͼ����
        if(mt9v03x_finish_flag)
        {
            ips114_show_string(0, 119, "time: ");
            ips114_show_uint(48, 119, system_getval_s(), 2);
            ips114_show_string(64, 119, "s");
            ips114_show_uint(72, 119, system_getval_ms()%1000, 3);
            ips114_show_string(96, 119, "ms");
            ips114_show_float(183, 119, valtage_now, 2, 3);
            if(image_count>=15)
            {
                Iteration_Threshould();
                image_count = 0;
                Thres_count++;
            }
            Binarization();
            image_draw_rectan();
            ips114_displayimage03x(Image_use[0], MT9V03X_W, MT9V03X_H);
            ips114_show_uint(207, 0, BlackThres, 3);
            ips114_show_uint(207, 16, Thres_count, 3);
            midline();
//            for(uint8 i = 0; i<120;i++)           //�����Ժ�ɫ��ʾ����Ļ��
//                ips114_draw_point(Mid_line[i], i, RGB565_RED);
            get_error();
            //err = 180-(Get_angle(Mid_line[12], 12, Mid_line[42], 42, Mid_line[72], 72)+Get_angle(Mid_line[18], 18, Mid_line[48], 48, Mid_line[78], 78)+Get_angle(Mid_line[15], 15, Mid_line[45], 45, Mid_line[75], 75))/3;
            //if(Mid_line[85]>Mid_line[25])
            //    err = -err;
            ips114_show_int(207, 32, error, 3);
            ips114_show_uint(207, 48, YERR, 3);
            ips114_show_uint(207, 64, far, 3);
            ips114_show_uint(207, 80, mid, 3);
            ips114_show_uint(207, 96, near, 3);
            mt9v03x_finish_flag = 0;
            image_count++;
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
        // �˴���д��Ҫѭ��ִ�еĴ���
}
#pragma section all restore
