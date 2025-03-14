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

// ����16λ�Ŀ������ÿ��16λ�Ŀ�������ͺ󣬺����ڲ�����ʱ33ms
void send_cmd_16bit(const u16 data)
{
    u8 i;
    if (0 == flag_is_dev_open)
    {
        return;
    }

#if 1 // ���ƽŰ��������Ĳ��ν��з������
    for (i = 0; i < 16; i++)
    {
        LED_CTL_PIN = 0;
        delay_100us();
        LED_CTL_PIN = 1;
        if ((data >> (15 - i)) & 0x01) // ������߼�1
        {
            delay_ms(3);
        }
        else // ������߼�0
        {
            delay_1_4ms();
        }
    }

    LED_CTL_PIN = 0;
    delay_100us();
    LED_CTL_PIN = 1;
    delay_ms(33);
#endif // ���ƽŰ��������Ĳ��ν��з������
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

    // P21PU = 1;
    // P21OE = 0;

    // �������ϵĽ�λ��
    P16PU = 1;
    P16OE = 0;

#else

    P15PU = 1; // ����
    P15OE = 0; // ����ģʽ

#endif

    // ����������ţ�

    flag_is_dev_open = 1;

    // ��EEPROM�ж�ȡ����

    while (1)
    { 
        // ��е��������ʱ������Ӧ�Ĳ���

        if (flag_is_recved_data)
        {
            if (ir_data == 0x08) // ON ����
            {

#if 1 // ���ƽŰ��������Ĳ��ν��з������
                LED_CTL_PIN = 1;
                delay_ms(2);
                LED_CTL_PIN = 0;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 1;
                delay_ms(3);
                LED_CTL_PIN = 0;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 1;
                delay_ms(4);
                LED_CTL_PIN = 0;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 1;
                delay_ms(5);
                LED_CTL_PIN = 0;
                delay_100us();
                delay_100us();
                LED_CTL_PIN = 1;
                delay_ms(51);

                // Ӧ�û��ɶ�Ӧ��ģʽ��

                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x08E0);
#endif // ���ƽŰ��������Ĳ��ν��з������
            }
            else if (ir_data == 0xC0) // OFF ����
            {
                // send_cmd_16bit(0x0C00);
                // delay_ms(40);
                // LED_CTL_PIN = 0;
            }
            else if (ir_data == 0x80) // AUTO ����
            {
                // send_cmd_16bit(0x0C00);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x0F11);
            }
            else if (ir_data == 0x60) // SPEED ����
            {
            }
            else if (0)  // R3C4
            {

            }
            else if (0)
            {
                
            }
            else
            {
                // �������������Գ���ռ�����Ż�
                u8 i;
                u16 tmp;
                for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
                {
                    if (table[i][0] == ir_data)
                    {
                        send_cmd_16bit(0x0C00); // ����ÿ��������Ҫ���������������
                        tmp = ((u16)table[i][1] << 8) + (u16)table[i][2];
                        send_cmd_16bit(tmp);
                        if (table[i][3] != UNUSE_VAL && table[i][4] != UNUSE_VAL)
                        {
                            send_cmd_16bit(tmp); // 4 * 16bit�Ŀ�������м�� 2 * 16bit����ͬ�ģ�����ֻ��Ҫ�ظ�����һ��
                            tmp = ((u16)table[i][3] << 8) + (u16)table[i][4];
                            send_cmd_16bit(tmp);
                        }
                        break;
                    }
                }
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
