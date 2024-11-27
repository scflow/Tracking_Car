/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 enableInterrupts(); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 disableInterrupts(); 来拒绝响应任何的中断，因此需要我们自己手动调用 enableInterrupts(); 来开启中断的响应。


// **************************** 代码区域 ****************************
int16  err;
int16  lasterr;
extern float valtage_now;
void core1_main(void)
{
    system_start();                         // 启动定时器，记录下当前的时间
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等
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
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips114_clear();
    BlackThres = 130;
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        last_error = error;
        //图像处理
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
//            for(uint8 i = 0; i<120;i++)           //中线以红色显示在屏幕上
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
        // 此处编写需要循环执行的代码
    }
        // 此处编写需要循环执行的代码
}
#pragma section all restore
