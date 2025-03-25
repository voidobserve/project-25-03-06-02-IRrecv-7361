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
// void EEPROM_Write_Data(u8 addr, u16 *data, u8 len)
// {
//     while (len)
//     {
//         EEPROM_Write_Byte(addr++, *data++);
//         len--;
//     }
// }

// ����16λ�Ŀ������ÿ��16λ�Ŀ�������ͺ󣬺����ڲ�����ʱ33ms
void send_cmd_16bit(const u16 data)
{
    u8 i;
    // if (0 == flag_is_dev_open)
    // {
    //     return;
    // }

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
    // if (0 == flag_is_dev_open)
    // {
    //     return;
    // }

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

/* DEFINE */
/// TODO:���ÿ��ģʽ��start��end������
#define MODE_AUTO_START (0)
#define MODE_AUTO_END (74)
#define MODE_FADE_START (0)
#define MODE_FADE_END (9)
#define MODE_JUMP_START (10)
#define MODE_JUMP_END (19)
#define MODE_ASYN_FADE_START (20)
#define MODE_ASYN_FADE_END (25)
#define MODE_ASYN_JUMP_START (26)
#define MODE_ASYN_JUMP_END (28)
#define MODE_TWINKLE_START (29)
#define MODE_TWINKLE_END (58)
#define MODE_STROBE_START (59)
#define MODE_STROBE_END (68)
#define MODE_IN_WAVES_START (69)
#define MODE_IN_WAVES_END (74)

#define MODE_R2C1_START (75)
#define MODE_R2C1_END (75)
#define MODE_R2C2_START (76)
#define MODE_R2C2_END (76)
#define MODE_R2C3_START (77)
#define MODE_R2C3_END (77)
#define MODE_R2C4_START (78)
#define MODE_R2C4_END (78)

#define MODE_R3C1_START (79)
#define MODE_R3C1_END (79)
#define MODE_R3C2_START (80)
#define MODE_R3C2_END (80)
#define MODE_R3C3_START (81)
#define MODE_R3C3_END (81)

#define MODE_R4C1_START (82)
#define MODE_R4C1_END (82)
#define MODE_R4C2_START (83)
#define MODE_R4C2_END (83)
#define MODE_R4C3_START (84)
#define MODE_R4C3_END (84)

#define MODE_R5C1_START (85)
#define MODE_R5C1_END (85)
#define MODE_R5C2_START (86)
#define MODE_R5C2_END (86)
#define MODE_R5C3_START (87)
#define MODE_R5C3_END (87)

#define MODE_R6C1_START (88)
#define MODE_R6C1_END (88)
#define MODE_R6C2_START (89)
#define MODE_R6C2_END (89)
#define MODE_R6C3_START (90)
#define MODE_R6C3_END (90)

/* enum */
// power_sta
enum
{
    POWER_STA_OFF = 0,
    POWER_STA_PRE_ON,
    POWER_STA_ON,
    POWER_STA_PRE_OFF,
};

/* params */
volatile u8 current_index;     // ��ǰ��������
volatile u8 start_index;       // ��ǰ������ʼ����
volatile u8 end_index;         // ��ǰ������������
volatile u16 next_frame_delay; // �´ν����ɫ�ĳ�ʱֵ

volatile u8 power_sta;              // ����״̬
volatile u8 current_speed;          // �ٶ�ֵ
volatile u8 current_static;         // ��ǰ��̬ģʽ
volatile u8 cur_mechanical_key_sta; // ��е������Ӧ��ģʽ

volatile u32 timer_count = 0;

/// TODO:������������const table���ݺͶε�start��endֵ
const u8 main_table[] = {

    // FADE
    [0] = 0x11, // [0]
    [1] = 0x22, // [1]
    [2] = 0x44, // [2]
    [3] = 0x33, // [3]
    [4] = 0x55, // [4]
    [5] = 0x66, // [5]
    [6] = 0x77, // [6]
    [7] = 0x88, // [7]
    [8] = 0xAA, // [8]
    [9] = 0xCC, // [9]

    // JUMP
    [10] = 0x11, // [10]
    [11] = 0x22, //
    [12] = 0x44, //
    [13] = 0x33, //
    [14] = 0x55, //
    [15] = 0x66, //
    [16] = 0x77, //
    [17] = 0x88, //
    [18] = 0xAA, //
    [19] = 0xCC, // [19]

    // ASYN_FADE
    [20] = 0x21, //
    [21] = 0x74, //
    [22] = 0x53, //
    [23] = 0x41, //
    [24] = 0x24, //
    [25] = 0x45, //

    // ASYN_JUMP
    [26] = 0xE0, //
    [27] = 0xA0, //
    [28] = 0xC0, //

    // TWINKLE
    [29] = 0xEE, //   // ע��ÿһ֡Ҫ����10��
    [30] = 0xEE, //
    [31] = 0xEE, //
    [32] = 0xEE, //
    [33] = 0xEE, //
    [34] = 0xEE, //
    [35] = 0xEE, //
    [36] = 0xEE, //
    [37] = 0xEE, //
    [38] = 0xEE, //
    [39] = 0xAA, //   // ע��ÿһ֡Ҫ����10��
    [40] = 0xAA, //
    [41] = 0xAA, //
    [42] = 0xAA, //
    [43] = 0xAA, //
    [44] = 0xAA, //
    [45] = 0xAA, //
    [46] = 0xAA, //
    [47] = 0xAA, //
    [48] = 0xAA, //
    [49] = 0xCC, //  // ע��ÿһ֡Ҫ����10��
    [50] = 0xCC, //
    [51] = 0xCC, //
    [52] = 0xCC, //
    [53] = 0xCC, //
    [54] = 0xCC, //
    [55] = 0xCC, //
    [56] = 0xCC, //
    [57] = 0xCC, //
    [58] = 0xCC, //

    // STROBE
    [59] = 0x17, //
    [60] = 0x27, //
    [61] = 0x47, //
    [62] = 0x37, //
    [63] = 0x57, //
    [64] = 0x67, //
    [65] = 0x77, //
    [66] = 0x87, //
    [67] = 0xA7, //
    [68] = 0xC7, //

    // IN_WAVES
    [69] = 0x11, //
    [70] = 0x12, //
    [71] = 0x22, //
    [72] = 0x24, //
    [73] = 0x44, //
    [74] = 0x14, //

    [75] = 0x11, // R2C1
    [76] = 0x22, // R2C2
    [77] = 0x44, // R2C3
    [78] = 0x77, // R2C4

    [79] = 0x33, /* R3C1 */
    [80] = 0x66, /* R3C2 */
    [81] = 0x55, /* R3C3 */
    [82] = 0x33, /* R4C1 */
    [83] = 0x66, /* R4C2 */
    [84] = 0x55, /* R4C3 */

    [85] = 0x33, /* R5C1 */
    [86] = 0x66, /* R5C2 */
    [87] = 0x55, /* R5C3 */
    [88] = 0x33, /* R6C1 */
    [89] = 0x66, /* R6C2 */
    [90] = 0x55, /* R6C3 */

    [91] = 0xEE,
    [92] = 0xAA, 
    [93] = 0xCC,
};

// FADE
#define SECTION_1_START (MODE_FADE_START)
#define SECTION_1_END (MODE_FADE_END)
const u8 section_1_speed_table[4] = {
    // FADE
    0x0F,
    0x0E,
    0x0D,
    0x0C,
};
const u16 section_1_delay_table[4] = {
    10000,
    7550,
    5155,
    2760,
};

// JUMP
#define SECTION_2_START (MODE_JUMP_START)
#define SECTION_2_END (MODE_JUMP_END)
const u8 section_2_speed_table[4] = {
    0x00,
};
const u16 section_2_delay_table[4] = {
    1960,
    964,
    465,
    165,
};

// ASYN_FADE
#define SECTION_3_START (MODE_ASYN_FADE_START)
#define SECTION_3_END (MODE_ASYN_FADE_END)
const u8 section_3_speed_table[4] = {
    0x08,
    0x0A,
    0x09,
    0x0B,
};
// const u16 section_3_delay_table[4] = {
//     4960,
//     4960,
//     4960,
//     4960,
// };

// ASYN_JUMP
#define SECTION_4_START (MODE_ASYN_JUMP_START)
#define SECTION_4_END (MODE_ASYN_JUMP_END)
// const u8 section_4_speed_table[4] = {
//     0x08,
//     0x0A,
//     0x09,
//     0x0B,
// };
// const u16 section_4_delay_table[4] = {
//     4960,
//     4960,
//     4960,
//     4960,
// };

// TWINKLE
#define SECTION_5_START (MODE_TWINKLE_START)
#define SECTION_5_END (MODE_TWINKLE_END)
const u8 section_5_speed_table[4] = {
    0x00,
};
const u16 section_5_delay_table[4] = {
    1960,
    964,
    465,
    165,
};

// STROBE
#define SECTION_6_START (MODE_STROBE_START)
#define SECTION_6_END (MODE_STROBE_END)
// const u8 section_6_speed_table[4] = {
//     0x08,
//     0x0A,
//     0x09,
//     0x0B,
// };
// const u16 section_6_delay_table[4] = {
//     4960,
//     4960,
//     4960,
//     4960,
// };

// IN_WAVES
#define SECTION_7_START (MODE_IN_WAVES_START)
#define SECTION_7_END (MODE_IN_WAVES_END)
const u8 section_7_speed_table[4] = {
    0x87,
    0x86,
    0x85,
    0x84, 
};
const u16 section_7_delay_table[4] = {
    5557,
    4560,
    3360,
    2000,
};

// STATIC
#define SECTION_8_START (75)
#define SECTION_8_END (77)
// const u8 section_8_speed_table[] = {
//     0,
//     0,
//     0,
//     0,
// };
// const u16 section_8_delay_table[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

// STATIC
#define SECTION_9_START (78)
#define SECTION_9_END (90)
// const u8 section_9_speed_table[] = {
//     0x10,
//     0x10,
//     0x10,
//     0x10,
// };
// const u16 section_9_delay_table[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

/**
 * @brief
 *
 * @param start �����������е���ʼλ��
 * @param end  �����������еĽ���λ��
 * @param own_data1  ����ܹ�����֡����Ҫ���ͣ������Ǵ����͵ĵڶ�֡���ݣ����������� 0xFFFF
 * @param own_data2  ����ܹ�����֡����Ҫ���ͣ������Ǵ����͵ĵ���֡���ݣ����������� 0xFFFF
 */
void key_event_same_deal(u8 start, u8 end, u16 own_data1, u16 own_data2)
{
    if (power_sta == POWER_STA_ON)
    {
        start_index = start;
        end_index = end;

        // ���浱ǰЧ����Ϣ
        EEPROM_Write_Byte(0, start);
        EEPROM_Write_Byte(1, end);
        EEPROM_Write_Byte(2, own_data1);
        EEPROM_Write_Byte(3, own_data2);
        EEPROM_Write_Byte(4, 0xA5A5);

        send_cmd_16bit(0x0C00);
        if (own_data1 != 0xFFFF)
            send_cmd_16bit(own_data1);
        if (own_data2 != 0xFFFF)
            send_cmd_16bit(own_data2);

        current_index = start_index; // �ӵ�һ֡��ʼ
        next_frame_delay = 0;        // ���̷��͵�һ֡����
    }
    else if (power_sta == POWER_STA_OFF)
    {
        // ���浱ǰЧ����Ϣ
        EEPROM_Write_Byte(0, start);
        EEPROM_Write_Byte(1, end);
        EEPROM_Write_Byte(2, own_data1);
        EEPROM_Write_Byte(3, own_data2);
        EEPROM_Write_Byte(4, 0xA5A5);
        power_sta = POWER_STA_PRE_ON;
    }
}

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

    // P15PU = 1; // ����
    P15OE = 0; // ����ģʽ

#endif

    // ����������ţ�
    // P16PU = 1; // ����
    P16OE = 0; // ����ģʽ

    // flag_is_dev_open = 1;

    // ��EEPROM�ж�ȡ����
    if (EEPROM_Read_Byte(4) != 0xA5A5)
    {
        // ���浱ǰЧ����Ϣ
        key_event_same_deal(MODE_AUTO_START, MODE_AUTO_END, 0x9645, 0x9645);
    }
    else
    { 
        power_sta = POWER_STA_PRE_ON;
    }

    while (1)
    {
        // ��е����ɨ��
        // if (0 == MECHANICAL_KEYING_PIN)
        // {
        //     u8 i;
        //     u8 cnt = 0;
        //     for (i = 0; i < 200; i++)
        //     {
        //         if (0 == MECHANICAL_KEYING_PIN)
        //         {
        //             cnt++;
        //         }
        //         delay_ms(1);
        //     }

        //     if (cnt >= 180)
        //     {
        //         flag_is_press_mechanical_keying = 1;
        //     }
        // }

        // - key deal
        if (flag_is_recved_data)
        {
            switch (ir_data)
            {
            case IR_KEY_ON:
                power_sta = POWER_STA_PRE_ON;
                break;

            case IR_KEY_OFF:
                power_sta = POWER_STA_PRE_OFF;
                break;

            case IR_KEY_AUTO:
                key_event_same_deal(MODE_AUTO_START, MODE_AUTO_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_AUTO;
                break;

            case IR_KEY_SPEED:
                if (power_sta != POWER_STA_OFF)
                {
                    current_speed++;
                    if (current_speed >= 4)
                        current_speed = 0;
                    current_index = start_index; // �ӵ�һ֡��ʼ
                    next_frame_delay = 0;        // ���̷��͵�һ֡����
                    send_cmd_16bit(0x0C00);
                }
                break;

            case IR_KEY_R3C4:
                current_static++;
                switch (current_static)
                {
                    /// TODO: ��Ҫ��ģʽ�������case����ÿ��caseӦ����ʾ����ɫ���޸ĺ�������
                default:
                    current_static = 0;
                case 0:
                    // ����ֱ���� main_table[] �����е����� 0x00EE
                    key_event_same_deal(91, 91, 0xFFFF, 0xFFFF);
                    break;
                case 1:
                    // ����ֱ���� main_table[] �����е����� 0x00AA
                    key_event_same_deal(92, 92, 0xFFFF, 0xFFFF);
                    break;
                case 2:
                    // ����ֱ���� main_table[] �����е����� 0x00CC
                    key_event_same_deal(93, 93, 0xFFFF, 0xFFFF);
                    break;
                }
                break;

            case IR_MECHANICAL_KEYING:
                switch (cur_mechanical_key_sta)
                {
                    /// TODO: ��Ҫ��ģʽ�������case����ÿ��caseӦ����ʾ����ɫ���޸ĺ�������
                default:
                    cur_mechanical_key_sta = 0;
                case 0: // AUTO
                    key_event_same_deal(MODE_AUTO_START, MODE_AUTO_END, 0x9645, 0x9645);
                    break;
                case 1: // ��
                    key_event_same_deal(MODE_R2C1_START, MODE_R2C1_END, 0xFFFF, 0xFFFF);
                    break;
                case 2: // ��
                    key_event_same_deal(MODE_R2C2_START, MODE_R2C2_END, 0xFFFF, 0xFFFF);
                    break;
                case 3: // ��
                    key_event_same_deal(MODE_R2C3_START, MODE_R2C3_END, 0xFFFF, 0xFFFF);
                    break;
                case 4: // ��
                    key_event_same_deal(MODE_R2C4_START, MODE_R2C4_END, 0x9645, 0x9645);
                    break;
                case 5: // FADE
                    key_event_same_deal(MODE_FADE_START, MODE_FADE_END, 0x9645, 0x9645);
                    break;
                case 6: // JUMP
                    key_event_same_deal(MODE_JUMP_START, MODE_JUMP_END, 0x9645, 0x9645);
                    break;

                case 7: // ASYN_FADE
                    key_event_same_deal(MODE_ASYN_FADE_START, MODE_ASYN_FADE_END, 0x9645, 0x9645);
                    break;

                case 8: // ASYN_JUMP
                    key_event_same_deal(MODE_ASYN_JUMP_START, MODE_ASYN_JUMP_END, 0x9645, 0x9645);
                    break;

                case 9: // TWINKLE
                    key_event_same_deal(MODE_TWINKLE_START, MODE_TWINKLE_END, 0x9645, 0x9645);
                    break;

                case 10: // STROBE
                    key_event_same_deal(MODE_STROBE_START, MODE_STROBE_END, 0x9645, 0x9645);
                    break;
                case 11: // IN_WAVES
                    key_event_same_deal(MODE_IN_WAVES_START, MODE_IN_WAVES_END, 0x9645, 0x9645);
                    break;

                case 12: // OFF
                    power_sta = POWER_STA_PRE_OFF;
                    break;
                }
                cur_mechanical_key_sta++;
                break;
            case IR_KEY_R4C4_FADE:
                key_event_same_deal(MODE_FADE_START, MODE_FADE_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R4C4_FADE;
                break;

            case IR_KEY_R5C4_JUMP:
                key_event_same_deal(MODE_JUMP_START, MODE_JUMP_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R5C4_JUMP;
                break;

            case IR_KEY_R6C4_ASYN_FADE:
                key_event_same_deal(MODE_ASYN_FADE_START, MODE_ASYN_FADE_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R6C4_ASYN_FADE;
                break;

            case IR_KEY_R7C1_IN_WAVES:
                key_event_same_deal(MODE_IN_WAVES_START, MODE_IN_WAVES_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R7C1_IN_WAVES;
                break;

            case IR_KEY_R7C2_STROBE:
                key_event_same_deal(MODE_STROBE_START, MODE_STROBE_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R7C2_STROBE;
                break;

            case IR_KEY_R7C3_TWINKLE:
                key_event_same_deal(MODE_TWINKLE_START, MODE_TWINKLE_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R7C3_TWINKLE;
                break;

            case IR_KEY_R7C4_ASYN_JUMP:
                key_event_same_deal(MODE_ASYN_JUMP_START, MODE_ASYN_JUMP_END, 0x9645, 0x9645);
                // cur_led_status = CUR_LED_STATUS_R7C4_ASYN_JUMP;
                break;

            case IR_KEY_R2C1:
                key_event_same_deal(MODE_R2C1_START, MODE_R2C1_END, 0xFFFF, 0xFFFF);
                // cur_led_status = CUR_LED_STATUS_R2C1_RED;
                break;
            case IR_KEY_R2C2:
                key_event_same_deal(MODE_R2C2_START, MODE_R2C2_END, 0xFFFF, 0xFFFF);
                // cur_led_status = CUR_LED_STATUS_R2C2_GREEN;
                break;
            case IR_KEY_R2C3:
                key_event_same_deal(MODE_R2C3_START, MODE_R2C3_END, 0xFFFF, 0xFFFF);
                // cur_led_status = CUR_LED_STATUS_R2C3_BLUE;
                break;
            case IR_KEY_R2C4:
                key_event_same_deal(MODE_R2C4_START, MODE_R2C4_END, 0x9645, 0x9645);
                break;
            case IR_KEY_R3C1:
                key_event_same_deal(MODE_R3C1_START, MODE_R3C1_END, 0x9F00, 0x9F00);
                break;
            case IR_KEY_R3C2:
                key_event_same_deal(MODE_R3C2_START, MODE_R3C2_END, 0x90F1, 0x90F1);
                break;
            case IR_KEY_R3C3:
                key_event_same_deal(MODE_R3C3_START, MODE_R3C3_END, 0x930F, 0x930F);
                break;
            case IR_KEY_R4C1:
                key_event_same_deal(MODE_R4C1_START, MODE_R4C1_END, 0x9F10, 0x9F10);
                break;
            case IR_KEY_R4C2:
                key_event_same_deal(MODE_R4C2_START, MODE_R4C2_END, 0x90F8, 0x90F8);
                break;
            case IR_KEY_R4C3:
                key_event_same_deal(MODE_R4C3_START, MODE_R4C3_END, 0x980F, 0x980F);
                break;
            case IR_KEY_R5C1:
                key_event_same_deal(MODE_R5C1_START, MODE_R5C1_END, 0x9F30, 0x9F30);
                break;
            case IR_KEY_R5C2:
                key_event_same_deal(MODE_R5C2_START, MODE_R5C2_END, 0x90FC, 0x90FC);
                break;
            case IR_KEY_R5C3:
                key_event_same_deal(MODE_R5C3_START, MODE_R5C3_END, 0x9F0F, 0x9F0F);
                break;
            case IR_KEY_R6C1:
                key_event_same_deal(MODE_R6C1_START, MODE_R6C1_END, 0x9FF0, 0x9FF0);
                break;
            case IR_KEY_R6C2:
                key_event_same_deal(MODE_R6C2_START, MODE_R6C2_END, 0x90FF, 0x90FF);
                break;
            case IR_KEY_R6C3:
                key_event_same_deal(MODE_R6C3_START, MODE_R6C3_END, 0x9F03, 0x9F03);
                break;

            case IR_KEY_4H:
                timer_count = 4 * 60 * 60 * 1000;
                // timer_count = 4 * 1000; // ������
                break;
            case IR_KEY_6H:
                timer_count = 6 * 60 * 60 * 1000;
                // timer_count = 6 * 1000; // ������
                break;
            case IR_KEY_8H:
                timer_count = 8 * 60 * 60 * 1000;
                // timer_count = 8 * 1000; // ������
                break;
            case IR_KEY_TIME_OFF:
                timer_count = 0;
                break;
            }

            // ir_data = 0x00;
            flag_is_recved_data = 0;
        }

        // - frame deal
        if (power_sta == POWER_STA_ON)
        {
            // - animation
            if (next_frame_delay == 0) // ��ʱ�ˣ�Ӧ�÷���֡����
            {
                u16 speed_code;

                if (current_index >= SECTION_1_START && current_index <= SECTION_1_END) // �ֶ�����ȡ���Ե��ٶ�code��֡���ʱ��
                {
                    speed_code = section_1_speed_table[current_speed];       // ��ȡ�ٶ�code
                    next_frame_delay = section_1_delay_table[current_speed]; // ��ȡ�����ñ�֡����֮֡�����ʱʱ��
                }
                else if (current_index >= SECTION_2_START && current_index <= SECTION_2_END)
                {
                    speed_code = section_2_speed_table[current_speed];
                    next_frame_delay = section_2_delay_table[current_speed];
                }
                else if (current_index >= SECTION_3_START && current_index <= SECTION_3_END)
                {
                    speed_code = section_3_speed_table[current_speed];
                    // next_frame_delay = section_3_delay_table[current_speed];
                    next_frame_delay = 4960;
                }
                else if (current_index >= SECTION_4_START && current_index <= SECTION_4_END)
                {
                    // speed_code = section_4_speed_table[current_speed];
                    // next_frame_delay = section_4_delay_table[current_speed];
                    speed_code = section_3_speed_table[current_speed];
                    // next_frame_delay = section_3_delay_table[current_speed];
                    next_frame_delay = 4960;
                }
                else if (current_index >= SECTION_5_START && current_index <= SECTION_5_END)
                {
                    speed_code = section_5_speed_table[current_speed];
                    next_frame_delay = section_5_delay_table[current_speed];
                }
                else if (current_index >= SECTION_6_START && current_index <= SECTION_6_END)
                {
                    // speed_code = section_6_speed_table[current_speed];
                    // next_frame_delay = section_6_delay_table[current_speed];
                    speed_code = section_3_speed_table[current_speed];
                    // next_frame_delay = section_3_delay_table[current_speed];
                    next_frame_delay = 4960;
                }
                else if (current_index >= SECTION_7_START && current_index <= SECTION_7_END)
                {
                    speed_code = section_7_speed_table[current_speed];
                    next_frame_delay = section_7_delay_table[current_speed];
                }
                // else if (current_index >= SECTION_8_START && current_index <= SECTION_8_END)
                // {
                //     // speed_code = section_8_speed_table[current_speed];
                //     // next_frame_delay = section_8_delay_table[current_speed];
                //     next_frame_delay = 0xFFFF;
                //     speed_code = 0x00;

                //     // next_frame_delay = 0xFFFF;
                //     // send_cmd_16bit_without_delay(XXXX);
                //     // continue;
                // }
                else if (current_index >= SECTION_9_START && current_index <= SECTION_9_END)
                {
                    // speed_code = section_9_speed_table[current_speed];
                    // next_frame_delay = section_9_delay_table[current_speed];

                    next_frame_delay = 0xFFFF;
                    speed_code = 0x10;
                    // send_cmd_16bit_without_delay(XXXX);
                    // continue;
                }
                else
                {
                    next_frame_delay = 0xFFFF;
                    speed_code = 0x00;
                }

                speed_code <<= 8;
                speed_code |= main_table[current_index];
                send_cmd_16bit(speed_code);

                current_index++;
                if (current_index > end_index)
                    current_index = start_index;
            }
        }
        else if (power_sta == POWER_STA_PRE_ON)
        {
            // - ���Ϳ�������
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

            power_sta = POWER_STA_ON;

            key_event_same_deal(EEPROM_Read_Byte(0), EEPROM_Read_Byte(1), EEPROM_Read_Byte(2), EEPROM_Read_Byte(3));
            next_frame_delay = 0;
            // next_frame_delay = 0xFFFF;
        }
        else if (power_sta == POWER_STA_PRE_OFF)
        {
            power_sta = POWER_STA_OFF;
            // - ���͹ػ�����
            send_cmd_16bit(0x0C00);
            send_cmd_16bit(0x0C00);
            delay_ms(40);
            LED_CTL_PIN = 0;
        }
    }
}

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

#if 1
        { // �������
            // static volatile u8 ir_fliter;
            static volatile u16 ir_level_cnt; // �����źŵ��½���ʱ��������
            static volatile u8 __ir_data;     //
            // static volatile u16 ir_long_press_cnt; // �z�y�t���b���L����Ӌ��ֵ

            // ��ÿ���½��ؽ��м���
            if (ir_level_cnt <= 1300)
                ir_level_cnt++;

            // ir_fliter <<= 1;
            // if (IR_RECV_PIN)
            // {
            //     ir_fliter |= 1;
            // }
            // ir_fliter &= 0x07;

            // if (ir_fliter == 0x07)
            //     filter_level = 1;
            // else if (ir_fliter == 0x00)
            //     filter_level = 0;

            // if (filter_level)
            if (IR_RECV_PIN)
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
                            // ir_long_press_cnt = 0;
                        }
#endif // ����У��İ汾
                    }
                    // else if (ir_level_cnt <= 1200) // С��120000,120ms,˵�����յ����ظ���
                    // {
                    //     // if (ir_long_press_cnt < 65535)
                    //     //     ir_long_press_cnt++;
                    // }
                    else // ����120000,˵�����յ���Ч������
                    {
                    }

                    ir_level_cnt = 0;
                }

                last_level_in_ir_pin = 0; // ��ʾ���յ����ǵ͵�ƽ
            }
        } // �������
#endif

        { // ��е�������
            static u16 key_press_cnt;
            if (0 == MECHANICAL_KEYING_PIN)
            {

                if (key_press_cnt == 10000) // ����
                {
                    ir_data = IR_KEY_OFF;
                    flag_is_recved_data = 1;
                }
                else if (key_press_cnt < 10000)
                {
                    key_press_cnt++;
                }
            }
            else // ̧��
            {
                if (key_press_cnt >= 100 &&key_press_cnt < 10000) // �̰�
                {
                    ir_data = IR_MECHANICAL_KEYING;
                    flag_is_recved_data = 1;
                }

                key_press_cnt = 0;
            }
        } // ��е�������
#if 0
        {
            static volatile u16 ir_level_cnt = 0; // �����źŵ��½���ʱ��������
            static volatile u8 __ir_data = 0;     //
            static volatile u16 ir_timeout = 0;
            static u8 count = 0;
            ir_level_cnt++;
            if (IR_RECV_PIN)
            {
                last_level_in_ir_pin = 1;

                if (++ir_timeout > 1200)
                {
                    count = 0;
                }
            }
            else
            {
                if (last_level_in_ir_pin)
                {
                    // �½���

                    if (ir_level_cnt < 18)
                    {
                        __ir_data <<= 1;
                        count++;
                    }
                    else if (ir_level_cnt <= 26)
                    {
                        __ir_data <<= 1;
                        __ir_data |= 0x01;
                        count++;
                    }

                    if (count == 24)
                    {
                        ir_data = __ir_data;
                        flag_is_recved_data = 1;
                    }
                    else if (count >= 32)
                    {
                        count = 0;
                    }

                    ir_level_cnt = 0;
                    ir_timeout = 0;
                }

                last_level_in_ir_pin = 0;
            }
        }
#endif
    }
    //=======T3========================
    if (T3IF & T3IE)
    {
        T3IF = 0;
        // ����ͨ����1ms����һ��

        {
            if (next_frame_delay > 0 && next_frame_delay < 0xFFFF)
            {
                next_frame_delay--;
            }
        }

        {
            if (timer_count)
            {
                timer_count--;
                if (timer_count == 0)
                {
                    ir_data = IR_KEY_OFF;
                    flag_is_recved_data = 1;
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
