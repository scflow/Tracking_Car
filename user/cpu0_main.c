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
* �ļ�����          cpu0_main
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
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************
#define MAX_DUTY            (100)                                               // ��� MAX_DUTY% ռ�ձ�
#define DIR_R               (P02_4)
#define PWM_R               (ATOM0_CH5_P02_5)

#define DIR_L               (P02_6)
#define PWM_L               (ATOM0_CH7_P02_7)

int8 duty = 0;
bool dir = true;

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50 )                                       // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

#define SERVO_MOTOR_L_MAX           (60 )                                       // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (120)                                       // ���������϶�����Χ �Ƕ�

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

#define VOLTAGE_PORT    ADC0_CH8_A8 // ������˿�

uint16 valtage_adc;
float valtage_now;

float servo_motor_duty = 90.0;                                                  // ��������Ƕ�
float servo_motor_dir = 1;
//PID p;
//p.kp = 5;
int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    system_start();                 // ������ʱ������¼�µ�ǰ��ʱ��
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 750);
    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_R, 17000, 0);                                                  // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0

    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_L, 17000, 0);                                                  // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0

    valtage_adc = 0;
    valtage_now = 0;
    duty = 12;
    adc_init(VOLTAGE_PORT, ADC_12BIT);                                          // �����Ŵ������� ADC1 ��ؼ���� ADC2
    printf("\r\nBattery voltage detection demo.");
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {

        valtage_adc = adc_convert(VOLTAGE_PORT);
        valtage_now = 13.2 * valtage_adc / 4096;                                // ������ɿƼ�ѧϰ�����ѹ���� ���Ӧ���ǵ�ص�ѹ 1/4 ��ô������� 13.2V

        duty=14-0.1*(0.2*(error)-0.2*(error-last_error));
        gpio_set_level(DIR_L, GPIO_HIGH);                                       // DIR����ߵ�ƽ
        pwm_set_duty(PWM_L, duty * (PWM_DUTY_MAX/100));                       // ����ռ�ձ�

        gpio_set_level(DIR_R, GPIO_HIGH);                                       // DIR����ߵ�ƽ
        pwm_set_duty(PWM_R,1.25*duty * (PWM_DUTY_MAX/100));                  // ����ռ�ձ�

        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty-0.7*(2.6*error-0.5*(last_error))+YERR*0.02));  // �������״̬
    }
}
#pragma section all restore


// **************************** �������� ****************************
