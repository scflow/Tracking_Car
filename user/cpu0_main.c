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
* 文件名称          cpu0_main
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
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
#define MAX_DUTY            (100)                                               // 最大 MAX_DUTY% 占空比
#define DIR_R               (P02_4)
#define PWM_R               (ATOM0_CH5_P02_5)

#define DIR_L               (P02_6)
#define PWM_L               (ATOM0_CH7_P02_7)

int8 duty = 0;
bool dir = true;

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (60 )                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (120)                                       // 定义主板上舵机活动范围 角度

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

#define VOLTAGE_PORT    ADC0_CH8_A8 // 定义检测端口

uint16 valtage_adc;
float valtage_now;

float servo_motor_duty = 90.0;                                                  // 舵机动作角度
float servo_motor_dir = 1;
//PID p;
//p.kp = 5;
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    system_start();                 // 启动定时器，记录下当前的时间
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 750);
    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    valtage_adc = 0;
    valtage_now = 0;
    duty = 12;
    adc_init(VOLTAGE_PORT, ADC_12BIT);                                          // 建议电磁传感器用 ADC1 电池检测用 ADC2
    printf("\r\nBattery voltage detection demo.");
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {

        valtage_adc = adc_convert(VOLTAGE_PORT);
        valtage_now = 13.2 * valtage_adc / 4096;                                // 根据逐飞科技学习主板分压计算 测得应该是电池电压 1/4 那么最大量程 13.2V

        duty=14-0.1*(0.2*(error)-0.2*(error-last_error));
        gpio_set_level(DIR_L, GPIO_HIGH);                                       // DIR输出高电平
        pwm_set_duty(PWM_L, duty * (PWM_DUTY_MAX/100));                       // 计算占空比

        gpio_set_level(DIR_R, GPIO_HIGH);                                       // DIR输出高电平
        pwm_set_duty(PWM_R,1.25*duty * (PWM_DUTY_MAX/100));                  // 计算占空比

        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty-0.7*(2.6*error-0.5*(last_error))+YERR*0.02));  // 舵机动作状态
    }
}
#pragma section all restore


// **************************** 代码区域 ****************************
