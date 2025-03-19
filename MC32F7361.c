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

/************************************************
;  *    @������            : EEPROM_Write_Byte
;  *    @˵��              : �ڲ�EEPROMд2���ֽ�
;  *    @�������          : ��ַ  ����
;  *    @���ز���          :
;  ***********************************************/
void EEPROM_Write_Byte(uint8_t addr, uint16_t data)
{
    HFEN = 1;
    EEAR = addr;
    EEDRH = (data >> 8) & 0xff;
    EEDRL = data & 0xff;
    GIE = 0;
    EEPR = 0x5A;
    EEPR = 0xA5; // �������
    EECR = 0x01; // ����EEPROM��д  д���ݲ���
    Nop();
    Nop();
    while (EEWRITE) // ��� EE д�����Ƿ����
    {
        // ClrWdt();                      //�忴�Ź�
    }
    GIE = 1;
}
/************************************************
;  *    @������            : EEPROM_Read_Byte
;  *    @˵��              : �ڲ�EEPROM��2���ֽ�
;  *    @�������          : ��ַ
;  *    @���ز���          : ����������
;  ***********************************************/
uint16_t EEPROM_Read_Byte(u8 addr)
{
    uint16_t EEDR;
    EEAR = addr;
    EECR = 0x02; // ���� EE ������
    Nop();
    Nop();
    while (EEREAD) // ��� EE д�����Ƿ����
    {
        // ClrWdt();                      //�忴�Ź�
    }
    EEDR = (EEDRH << 8) | EEDRL;
    return EEDR;
}
/************************************************
;  *    @������            : EEPROM_Write_Data
;  *    @˵��              : ����д��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void EEPROM_Write_Data(u8 addr, u16 *data, u8 len)
{
    while (len)
    {
        EEPROM_Write_Byte(addr++, *data++);
        len--;
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

// ����16λ�Ŀ������ÿ��16λ�Ŀ�������ͺ󣬺����ڲ�������ʱ
void send_cmd_16bit_without_delay(const u16 data)
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

void TIMER3_INT_Init(void)
{
    T3LOAD = 135 - 1;                               // ��Ƶ��������1ms����һ���жϣ��ü��������ֵ���������������һЩ������
    T3CR = (0x01 << 7) | (0x01 << 2) | (0x01 << 1); // ʹ�ܶ�ʱ����ʱ��Դѡ��FCPU��64��Ƶ
    T3IE = 1;
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
    TIMER3_INT_Init();
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
    delay_ms(1); // �ȴ�ϵͳ�ȶ�

// �����źŽ������ţ�
#if USE_MY_DEBUG

    P21PU = 1;
    P21OE = 0;

    // // �������ϵĽ�λ��
    // P16PU = 1;
    // P16OE = 0;

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
            if (ir_data == IR_KEY_ON) // ON ����
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

                // Ӧ�û��ɶ�Ӧ��ģʽ�����ݣ�

                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x08E0);
#endif // ���ƽŰ��������Ĳ��ν��з������
            }
            else if (ir_data == IR_KEY_OFF) // OFF ����
            {
                // send_cmd_16bit(0x0C00);
                // delay_ms(40);
                // LED_CTL_PIN = 0;
            }
            // else if (ir_data == IR_KEY_AUTO) // AUTO ����
            // {
            //     // send_cmd_16bit(0x0C00);
            //     // send_cmd_16bit(0x9645);
            //     // send_cmd_16bit(0x9645);
            //     // send_cmd_16bit(0x0F11);
            // }
            else if (ir_data == IR_KEY_SPEED) // SPEED ����
            {
                // ����Ӧ�ı�־λ��һ���ڶ�Ӧ��ģʽ����ɨ�账��
                flag_is_press_speed = 1;
            }
            else if (ir_data == IR_KEY_R3C4) // R3C4��ÿ�ΰ���ʱ��ı䵱ǰ��ʾ����ɫ
            {
                u16 tmp;
                if (cur_r3c4_status == R3C4_STATUS_1)
                {
                    tmp = 0x00EE;
                    cur_r3c4_status = R3C4_STATUS_2;
                }
                else if (cur_r3c4_status == R3C4_STATUS_2)
                {
                    tmp = 0x00AA;
                    cur_r3c4_status = R3C4_STATUS_3;
                }
                else if (cur_r3c4_status == R3C4_STATUS_3)
                {
                    tmp = 0x00CC;
                    cur_r3c4_status = R3C4_STATUS_1;
                }

                // ����ǰ��״̬д��EEPROM���ٴ��ϵ��ͽ����״̬
                // ����ǰR3C4��״̬д��EEPROM���ٴ��ϵ��ͽ���R3C4��Ӧ����״̬

                send_cmd_16bit(0x0C00);
                send_cmd_16bit(tmp);
            }
            else if (ir_data == IR_KEY_R4C4_FADE)
            {
                // cur_led_status = CUR_LED_STATUS_AUTO;
            }
            else
            {
                // �������������Գ���ռ�����Ż�
                u8 i;
                u16 tmp;

                if (IR_KEY_AUTO == ir_data) // AUTO ����
                {
                    cur_led_status = CUR_LED_STATUS_AUTO; // ��ʾ��ǰ���� AUTO ģʽ

                    // ���autoģʽ��ʹ�õ��ı���
                    // auto_mode_info = 0;
                    auto_mode_info.cur_submode_index = 0;
                    auto_mode_info.delay_time = 0;
                    auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME; // autoģʽ�տ�ʼ����Ҫ�ȴ�һ��ʱ���ٷ���һ֡����
                }
                else
                {
                    // �������������
                    cur_led_status = CUR_LED_STATUS_SINGLE_COLOR;
                }

                for (i = 0; i < sizeof(table_irkey) / sizeof(table_irkey[0]); i++)
                {
                    if (table_irkey[i][0] == ir_data)
                    {
                        send_cmd_16bit(0x0C00); // ����ÿ��������Ҫ���������������
                        tmp = ((u16)table_irkey[i][1] << 8) + (u16)table_irkey[i][2];
                        send_cmd_16bit(tmp);
                        if (table_irkey[i][3] != UNUSE_VAL && table_irkey[i][4] != UNUSE_VAL)
                        {
                            send_cmd_16bit(tmp); // 4 * 16bit�Ŀ�������м�� 2 * 16bit����ͬ�ģ�����ֻ��Ҫ�ظ�����һ��
                            tmp = ((u16)table_irkey[i][3] << 8) + (u16)table_irkey[i][4];
                            send_cmd_16bit(tmp);
                        }
                        break;
                    }
                }
            }

            ir_data = 0;
            flag_is_recved_data = 0;
        } // if (flag_is_recved_data)

        if (CUR_LED_STATUS_AUTO == cur_led_status) // �����ǰ���� AUTO ģʽ
        {
            // ����Ƿ��а��� SPEED������а��£��ı�autoģʽ����ģʽ��
            if (flag_is_press_speed)
            {
                flag_is_press_speed = 0;

                { // ���Ͱ���SPEED�����󣬶�Ӧ������֡
                    u8 tmp;
                    send_cmd_16bit(0x0C00); // ������ģʽ�£���⵽��SPEED���£������ȷ������
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        tmp = 0x0F;
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        tmp = 0x0E;
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        tmp = 0x0D;
                    }
                    // else if (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        tmp = 0x0C;
                    }

                    send_cmd_16bit(((u32)tmp << 8) | 0x11);
                } // ���Ͱ���SPEED�����󣬶�Ӧ������֡

                auto_mode_info.cur_auto_submode++;
                if (auto_mode_info.cur_auto_submode > CUR_AUTO_SUBMODE_4)
                {
                    auto_mode_info.cur_auto_submode = CUR_AUTO_SUBMODE_1;
                }

                // ����а��� SPEED���л����ݷ������ڣ���յ�ǰ���ݷ������ڵļ���
                // ���autoģʽʹ�õ��ļ�������
                // auto_mode_info = {0};
                // auto_mode_info.cur_auto_submode = 0;
                auto_mode_info.cur_submode_index = 0;
                // auto_mode_info.delay_time = 0;
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME; // ����SPEED��Ҫ��������һ�η���ʱ����

                // ����Ӧ����ģʽд��EEPROM ========================================

                delay_ms(2); // �ȴ���ʱ�������Ӧ�ı���
            } // if (flag_is_press_speed)
            // ����Ƿ��а��� SPEED������а��£��ı�autoģʽ����ģʽ

            // ���÷���ʱ������
            if (CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME == auto_mode_info.cur_submode_status)
            {
                /*
                    һ��autoģʽ�տ�ʼ�������е�0~8֡���ݵ���ʱʱ�䣬
                    ��ģʽ1�� 3s
                    ��ģʽ2��10s
                    ��ģʽ3��7.55s
                    ��ģʽ4��5s
                */
                u8 index;
                if (auto_mode_info.cur_submode_index > 0)
                {
                    index = auto_mode_info.cur_submode_index - 1;
                }
                else
                {
                    index = 0;
                }

                // if (auto_mode_info.cur_submode_index <= 8)
                if (index <= 8)
                {
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        // auto_mode_info.delay_time = 3000;
                        auto_mode_info.delay_time = 2760;
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 10000;
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 7550;
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        auto_mode_info.delay_time = 5000;
                    }
                }
                // else if (auto_mode_info.cur_submode_index == 9 ||
                //          auto_mode_info.cur_submode_index == 20 ||
                //          auto_mode_info.cur_submode_index == 27 ||
                //          auto_mode_info.cur_submode_index == 31 ||
                //          auto_mode_info.cur_submode_index == 62 ||
                //          auto_mode_info.cur_submode_index == 73 ||
                //          auto_mode_info.cur_submode_index == 80)
                else if (index == 9 ||
                         index == 20 ||
                         index == 27 ||
                         index == 31 ||
                         index == 62 ||
                         index == 73 ||
                         index == 80)
                {
                    /*
                        ���ݷ��ͺ󣬹̶���ʱ33ms������֡�±�
                    */
                    auto_mode_info.delay_time = 33;
                }
                // else if ((auto_mode_info.cur_submode_index >= 10 && auto_mode_info.cur_submode_index <= 19) ||
                //          (auto_mode_info.cur_submode_index >= 32 && auto_mode_info.cur_submode_index <= 61))
                else if ((index >= 10 && index <= 19) ||
                         (index >= 32 && index <= 61))
                {
                    /*
                        �����е� 10 ~ 19 ֡���� 32 ~ 61 ֡
                        ��ģʽ1�� 172ms
                        ��ģʽ2�� 2s
                        ��ģʽ3�� 1s
                        ��ģʽ4�� 470ms
                    */
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 172;
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 2000;
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 1000;
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        auto_mode_info.delay_time = 470;
                    }
                }
                // else if ((auto_mode_info.cur_submode_index >= 21 && auto_mode_info.cur_submode_index <= 26) ||
                //          (auto_mode_info.cur_submode_index >= 28 && auto_mode_info.cur_submode_index <= 30) ||
                //          (auto_mode_info.cur_submode_index >= 63 && auto_mode_info.cur_submode_index <= 72))
                else if ((index >= 21 && index <= 26) ||
                         (index >= 28 && index <= 30) ||
                         (index >= 63 && index <= 72))
                {
                    // �����е� 21 ~ 26֡��28~30֡��63~72֡���̶���ʱ5s
                    auto_mode_info.delay_time = 5000;
                }
                // else if (auto_mode_info.cur_submode_index >= 74 && auto_mode_info.cur_submode_index <= 79)
                else if (index >= 74 && index <= 79)
                {
                    /*
                        �����е� 74 ~ 79֡��
                        ��ģʽ1�� 2s
                        ��ģʽ2��5.55s
                        ��ģʽ3��4.56s
                        ��ģʽ4��470ms
                    */
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 2000;
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 5550;
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 4560;
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        auto_mode_info.delay_time = 470;
                    }
                }
                // else if (auto_mode_info.cur_submode_index == 81)
                else if (index == 81)
                {
                    /*
                        ��81֡�����һ֡��
                        ��ģʽ1�� 3s
                        ��ģʽ2�� 10s
                        ��ģʽ3�� 1s
                        ��ģʽ4�� 5s
                    */
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 3000;
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 10000;
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        auto_mode_info.delay_time = 1000;
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        auto_mode_info.delay_time = 5000;
                    }
                }

                // �����귢��ʱ�����󣬽���ȴ�ģʽ
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_STANDBY;
            }
            // if (CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME == auto_mode_info.cur_submode_status)
            // ���÷���ʱ����

            // ����Ѿ�׼����������:
            if (CUR_SUBMODE_STATUS_READY_TO_SEND == auto_mode_info.cur_submode_status)
            {
                u16 tmp = ((u16)table_auto_in_submode[auto_mode_info.cur_submode_index][0] << 8) | table_auto_in_submode[auto_mode_info.cur_submode_index][1];

                // ���ݵ�ǰ���͵�����֡�±꣬������ģʽ�ĸ�����ͬ�����ݣ�
                if (auto_mode_info.cur_submode_index <= 8)
                {
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0C << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0F << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0E << 8);
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        tmp |= (0x0D << 8);
                    }
                }
                else if ((auto_mode_info.cur_submode_index >= 21 && auto_mode_info.cur_submode_index <= 26) ||
                         (auto_mode_info.cur_submode_index >= 28 && auto_mode_info.cur_submode_index <= 30) ||
                         (auto_mode_info.cur_submode_index >= 63 && auto_mode_info.cur_submode_index <= 72))
                {
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0A << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x09 << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0B << 8);
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        tmp |= (0x08 << 8);
                    }
                }
                else if (auto_mode_info.cur_submode_index >= 74 && auto_mode_info.cur_submode_index <= 79)
                {
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x04 << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x07 << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x06 << 8);
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        // ���ģʽ�¾���0�����ö����޸�
                    }
                }
                else if (auto_mode_info.cur_submode_index == 81)
                {
                    if (CUR_AUTO_SUBMODE_1 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0C << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_2 == auto_mode_info.cur_auto_submode)
                    {
                        tmp |= (0x0F << 8);
                    }
                    else if (CUR_AUTO_SUBMODE_3 == auto_mode_info.cur_auto_submode)
                    {
                        // ���ģʽ�¾���0�����ö����޸�
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        tmp |= (0x08  << 8);
                    }
                }
                // ���ݵ�ǰ���͵�����֡�±꣬������ģʽ�ĸ�����ͬ�����ݣ�

                send_cmd_16bit_without_delay(tmp);
                // ������ɺ��±��һ
                auto_mode_info.cur_submode_index++;
                if (auto_mode_info.cur_submode_index >= ARRAY_SIZE(table_auto_in_submode))
                {
                    // ����±�Խ��
                    auto_mode_info.cur_submode_index = 0;
                }

                // ����δȷ��Ҫ��Ҫ�ŵ�ǰ�棬����Ҫ�ϻ�ʵ�ʲ��ԣ���һ���Ҫ�ŵ���ǰ�棬����Ҫ�ŵ�����ǰ�棬��Ϊ�����ķ���ʱ�����Ǵӷ���֡��һ�̿�ʼ����һ֡����ʼλ��:
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME;
            }
        }
        else // ������autoģʽ�����autoģʽʹ�õ��ı��������Բ���գ�
        {
        }

    } // while (1)
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
    // if (T0IF & T0IE)
    // {
    //     P10D = !P10D;
    //     T0IF = 0;
    // }
    //=======T1========================
    // if (T1IF & T1IE)
    // {
    //     P11D = !P11D;
    //     T1IF = 0;
    // }
    //=======T2========================
    if (T2IF & T2IE)
    {
        T2IF = 0;
        // ÿ��100us����һ��
        { // �������
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
        } // �������

        { // 1ms����һ��
            static u8 cnt_100us;
            cnt_100us++;
            if (cnt_100us >= 10) // ������� 1ms
            {
                cnt_100us = 0;

                { // AUTO ģʽ�£����Ʒ������ݵ�����
                    static u16 auto_period_cnt;
                    if (cur_led_status == CUR_LED_STATUS_AUTO) // �����ǰ���� AUTO ģʽ
                    {
                        u8 flag_is_period_arrive; // �����Ƿ����ı�־λ
                        auto_period_cnt++;
                        // if ((cur_auto_period == CUR_AUTO_PERIOD_3S && auto_period_cnt >= 3000) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_10S && auto_period_cnt >= 10000) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_7_5S && auto_period_cnt >= 7500) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_5S && auto_period_cnt >= 5000))
                        // {
                        //     flag_is_period_arrive = 1;
                        // }
                        // else // ��������δ������
                        // {
                        //     flag_is_period_arrive = 0;
                        // }

                        if (flag_is_period_arrive) // ������˷������ݵ�����
                        {
                        }
                    }
                    else // ������ AUTO ģʽ
                    {
                        auto_period_cnt = 0;
                    }
                } // AUTO ģʽ�£����Ʒ������ݵ�����
            }

        } // 1ms����һ��
    }
    //=======T3========================
    if (T3IF & T3IE)
    {
        T3IF = 0;
        // ����ͨ����1ms����һ��

        if (CUR_LED_STATUS_AUTO == cur_led_status) // �����ǰ����AUTOģʽ
        {
            if (CUR_SUBMODE_STATUS_STANDBY == auto_mode_info.cur_submode_status) // ������ڵȴ�����ʱ����
            {
                if (auto_mode_info.delay_time > 0)
                {
                    auto_mode_info.delay_time--;
                }

                if (auto_mode_info.delay_time == 0)
                {
                    // ����ȴ����ͼ���ĵ���ʱ����
                    auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_READY_TO_SEND;
                }
            }
        }
    }
    __asm;
    swapar _statusbuf;
    movra _PFLAG;
    swapr _abuf;
    swapar _abuf;
    __endasm;
}

/**************************** end of file *********************************************/
