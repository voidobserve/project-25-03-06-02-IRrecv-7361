/******************************************************************************
;  *       @�ͺ�                 : MC32F7361
;  *       @��������             : 2021.12.21
;  *       @��˾/����            : SINOMCU-FAE
;  *       @����΢����֧��       : 2048615934
;  *       @����΢����           : http://www.sinomcu.com/
;  *       @��Ȩ                 : 2021 SINOMCU��˾��Ȩ����.
;  *----------------------ժҪ����---------------------------------
;  *                ��ʱ��
;  *                T0   P10 2mS��ת
;  *                T1   P11 1mS��ת
;  *                T2   P12 100uS��ת
;  *                T3   P14 50uS��ת
******************************************************************************/

#include "user.h"

// 100us��ʱ��ǰ������ �� FCPU = FHOSC / 4
void delay_100us(void)
{
    u8 i = 74;
    while (i--)
    {
        Nop();
    }
}

// 200us��ʱ��ǰ������ �� FCPU = FHOSC / 4
void delay_200us(void)
{
    u8 i = 150;
    while (i--)
    {
        Nop();
    }
}

// 1.4ms��ʱ
void delay_1_4ms(void)
{
    u8 i;
    for (i = 0; i < 14; i++)
    {
        delay_100us();
    }
}

// �����ź����� ���ɵ͵�ƽ���� + 28ms�ߵ�ƽ + ���ɵ͵�ƽ����
void send_cmd(const u8 data)
{
    u8 i;

    if (0 == flag_is_dev_open)
    {
        return;
    }

    for (i = 0; i < 8; i++)
    {
        LED_CTL_PIN = 1;
        delay_100us();
        LED_CTL_PIN = 0;
        if ((data >> (7 - i)) & 0x01) // ������߼�1
        {

            delay_ms(3);
        }
        else // ������߼�0
        {
            delay_1_4ms();
        }
    }

    LED_CTL_PIN = 1;
    delay_100us();
    LED_CTL_PIN = 0;
    delay_ms(33);
}

/************************************************
;  *    @������          : CLR_RAM
;  *    @˵��            : ��RAM
;  *    @�������        :
;  *    @���ز���        :
;  ***********************************************/
void CLR_RAM(void)
{
    for (FSR0 = 0; FSR0 < 0xff; FSR0++)
    {
        INDF0 = 0x00;
    }
    FSR0 = 0xFF;
    INDF0 = 0x00;
}
/************************************************
;  *    @������            : IO_Init
;  *    @˵��              : IO��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void IO_Init(void)
{
    IOP0 = 0x00;   // io������λ
    OEP0 = 0x3F;   // io�ڷ��� 1:out  0:in
    PUP0 = 0x00;   // io����������   1:enable  0:disable
    PDP0 = 0x00;   // io����������   1:enable  0:disable
    P0ADCR = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

    IOP1 = 0x00;   // io������λ
    OEP1 = 0xFF;   // io�ڷ��� 1:out  0:in
    PUP1 = 0x00;   // io����������   1:enable  0:disable
    PDP1 = 0x00;   // io����������   1:enable  0:disable
    P1ADCR = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

    IOP2 = 0x00; // io������λ
    OEP2 = 0x0F; // io�ڷ��� 1:out  0:in
    PUP2 = 0x00; // io����������   1:enable  0:disable
    PDP2 = 0x00; // io����������   1:enable  0:disablea

    PMOD = 0x00;  // P00��P01��P13 io�˿�ֵ�ӼĴ��������������
    DRVCR = 0x80; // ��ͨ����
}

/************************************************
;  *    @������            : TIMER2_INT_Init
;  *    @˵��              : ��ʱ��2��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void TIMER2_INT_Init(void)
{
    T2CR = DEF_SET_BIT0 | DEF_SET_BIT1; // CPU,8��Ƶ
    T2CNT = 100 - 1;
    T2LOAD = 100 - 1; // 100us
    T2EN = 1;
    T2IE = 1;
}
/************************************************
;  *    @������            : Sys_Init
;  *    @˵��              : ϵͳ��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void Sys_Init(void)
{
    GIE = 0;
    CLR_RAM();
    IO_Init();
    // TIMER0_INT_Init();
    // TIMER1_INT_Init();
    TIMER2_INT_Init();
    // TIMER3_INT_Init();
    GIE = 1;
}

void led_mode_handle(u8 mode)
{
    u8 tmp = cur_led_status;
    // cur_led_status = rf_data;

    if (mode == 0x01)
    {
        LED_CTL_PIN = 1;
        delay_ms(5);
        send_cmd(1);
        flag_is_dev_open = 1; // �򿪵ƹ�
    }
    else if (mode == 0x03)
    {
        LED_CTL_PIN = 0;
        flag_is_dev_open = 0; // �رյƹ�
    }
    else if (mode == 0x04)
    {
        send_cmd(1);
    }
    else if (mode == 0x06)
    {
        send_cmd(10);
    }
    else if (mode == 0x07)
    {
        send_cmd(2);
    }
    else if (mode == 0x09)
    {
        send_cmd(9);
    }
    else if (mode == 0x0A)
    {
        send_cmd(3);
    }
    else if (mode == 0x0C)
    {
        send_cmd(8);
    }
    else if (mode == 0x0D)
    {
        send_cmd(4);
    }
    else if (mode == 0x0F)
    {
        send_cmd(7);
    }
    else if (mode == 0x10)
    {
        send_cmd(5);
    }
    else if (mode == 0x12)
    {
        send_cmd(6);
    }
    else
    {
        // �������Ч�ļ�ֵ
        cur_led_status = tmp; // �õƹ��״̬����
    }
}

/************************************************
;  *    @������            : main
;  *    @˵��              : ������
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void main(void)
{
    Sys_Init();

// �����źŽ������ţ�
#if USE_MY_DEBUG

    P21PU = 1;
    P21OE = 0;

#else

    P15PU = 1; // ����
    P15OE = 0; // ����ģʽ

#endif

    // ����������ţ�

    flag_is_dev_open = 1;

    while (1)
    {
        // DEBUG_PIN = ~DEBUG_PIN;
        // // delay_200us();
        // delay_1_4ms();

        // send_cmd(0x96);
        // send_cmd(0x45);

        // �ǰ�������ʱ������Ӧ�Ĳ��������ǰ����ɿ�ʱ����Ӧ����

        if (flag_is_recved_data)
        {
            if (ir_data == 0x08)
            {
                LED_CTL_PIN = 0;
                delay_ms(2);
                LED_CTL_PIN = 1;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 0;
                delay_ms(3);
                LED_CTL_PIN = 1;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 0;
                delay_ms(4);
                LED_CTL_PIN = 1;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 0;
                delay_ms(5);
                LED_CTL_PIN = 1;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 0;
                delay_ms(51);
                send_cmd(0x96);
                send_cmd(0x45);

                send_cmd(0x96);
                send_cmd(0x45);

                send_cmd(0x08);
                send_cmd(0xE0);
            }
            else if (ir_data == 0x90) // R
            {
                send_cmd(0x0C);
                send_cmd(0x00);
                send_cmd(0x00);
                send_cmd(0x11);
            }
            else if (ir_data == 0xB8) // G
            {
                send_cmd(0x0C);
                send_cmd(0x00);
                send_cmd(0x00);
                send_cmd(0x22);
            }
            else if (ir_data == 0xF8) // B
            {
                send_cmd(0x0C);
                send_cmd(0x00);
                send_cmd(0x00);
                send_cmd(0x44);
            }

            ir_data = 0;
            flag_is_recved_data = 0;
        }
    }
}
/************************************************
;  *    @������            : interrupt
;  *    @˵��              : �жϺ���
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void int_isr(void) __interrupt
{
    __asm;
    movra _abuf;
    swapar _PFLAG;
    movra _statusbuf;
    __endasm;
    //=======T0========================
    if (T0IF & T0IE)
    {
        P10D = !P10D;
        T0IF = 0;
    }
    //=======T1========================
    if (T1IF & T1IE)
    {
        P11D = !P11D;
        T1IF = 0;
    }
    //=======T2========================
    if (T2IF & T2IE)
    {
        T2IF = 0;

        static volatile u8 ir_fliter;
        static volatile u16 ir_level_cnt;      // �����źŵ��½���ʱ��������
        static volatile u8 __ir_data;          //
        static volatile u16 ir_long_press_cnt; // �z�y�t���b���L����Ӌ��ֵ

        // ��ÿ���½��ؽ��м���
        ir_level_cnt++;

        ir_fliter <<= 1;
        if (IR_RECV_PIN)
        {
            ir_fliter |= 1;
        }
        ir_fliter &= 0x07;

        if (ir_fliter == 0x07)
            filter_level = 1;
        else if (ir_fliter == 0x00)
            filter_level = 0;

        if (filter_level)
        {
            last_level_in_ir_pin = 1; // ��ʾ���յ����Ǹߵ�ƽ

            // ���֮ǰҲ�Ǹߵ�ƽ
            if (ir_level_cnt > 1200) // ��ʱʱ��(120000us,120ms)
            {
                // if (__ir_data != 0) // ��ʱ���ҽ��յ�����(������մ������л��ir_data����)
                if (__ir_data != 0) // ��ʱ���ҽ��յ�����(���������жϷ������а�__ir_data�Զ�����)
                {
                    // // ��У��İ汾��
                    // if ((u8)(__ir_data >> 8) == (u8)(~__ir_data)) // �����ֵ��ԭ��ͷ������
                    // {
                    // flag_is_recved_data = 1;
                    // }

                    // ����У��İ汾
                    if (0 == flag_is_recved_data)
                    {
                        ir_data = ~__ir_data;
                        __ir_data = 0;
                        flag_is_recved_data = 1;
                    }
                }

                flag_is_recv_ir_repeat_code = 0; // ��Ϊң���������Ѿ����£�Ȼ���ɿ�
            }
        }
        else
        {
            if (last_level_in_ir_pin)
            {
                // ���֮ǰ��⵽���Ǹߵ�ƽ�����ڼ�⵽�˵͵�ƽ
                if (ir_level_cnt <= 8) // С��800us��˵�����յ���Ч�����ݣ����½���
                {
                    // FLAG_IS_RECVED_ERR_IR_DATA = 1;
                    // flag_is_recv_ir_repeat_code = 0;
                }
                else if (ir_level_cnt <= 18) // С��1800us,˵�����յ����߼�0
                {
                    __ir_data <<= 1;

                    // P15D = 0; // ���Ժ������
                    // P15D = ~P15D; // ���Ժ������
                }
                else if (ir_level_cnt <= 26) // С��2600us,˵�����յ����߼�1
                {
                    __ir_data <<= 1;
                    __ir_data |= 0x01;

                    // P15D = 1; // ���Ժ������
                }
                else if (ir_level_cnt <= 150) // С��15000us,˵�����յ��˸�ʽͷ
                {
                    // FLAG_IS_RECVED_ERR_IR_DATA = 1;
                    // ir_long_press_cnt = 0; // ������һ�����ⲻ������
                }
                else if (ir_level_cnt <= 420) // С��42ms,˵��������һ�������ĺ����ź�
                {
#if 0 // ��У��İ汾������Դ������������У��

            if ((u8)(__ir_data >> 8) == (u8)(~__ir_data)) // �����ֵ��ԭ��ͷ������
            {
                flag_is_recved_data = 1;
                flag_is_recv_ir_repeat_code = 1; //
                ir_long_press_cnt = 0;
            }
#else // ����У��İ汾
                    if (0 == flag_is_recved_data)
                    {
                        ir_data = ~__ir_data;
                        __ir_data = 0;
                        flag_is_recved_data = 1;
                        flag_is_recv_ir_repeat_code = 1; //
                        ir_long_press_cnt = 0;
                    }
#endif // ����У��İ汾
                }
                else if (ir_level_cnt <= 1200) // С��120000,120ms,˵�����յ����ظ���
                {
                    ir_long_press_cnt++;
                }
                else // ����120000,˵�����յ���Ч������
                {
                }

                ir_level_cnt = 0;
            }

            last_level_in_ir_pin = 0; // ��ʾ���յ����ǵ͵�ƽ
        }
    }
    //=======T3========================
    if (T3IF & T3IE)
    {
        T3IF = 0;
    }
    __asm;
    swapar _statusbuf;
    movra _PFLAG;
    swapr _abuf;
    swapar _abuf;
    __endasm;
}

/**************************** end of file *********************************************/
