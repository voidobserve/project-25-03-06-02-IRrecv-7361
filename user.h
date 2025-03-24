/******************************************************************************
;  *       @�ͺ�                   : MC32F7361
;  *       @��������               : 2021.12.21
;  *       @��˾/����              : SINOMCU-FAE
;  *       @����΢����֧��         : 2048615934
;  *       @����΢����             : http://www.sinomcu.com/
;  *       @��Ȩ                   : 2021 SINOMCU��˾��Ȩ����.
;  *---------------------- ���� ---------------------------------
;  *                   ��������ʱʹ��ȫ�ֱ���
******************************************************************************/
#ifndef USER
#define USER
#include "mc32-common.h"
#include "MC32F7361.h"

/*****************************************************************
;       Function : Define variables
;*****************************************************************/

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

#define DEF_SET_BIT0 0x01
#define DEF_SET_BIT1 0x02
#define DEF_SET_BIT2 0x04
#define DEF_SET_BIT3 0x08
#define DEF_SET_BIT4 0x10
#define DEF_SET_BIT5 0x20
#define DEF_SET_BIT6 0x40
#define DEF_SET_BIT7 0x80

#define DEF_CLR_BIT0 0xFE
#define DEF_CLR_BIT1 0xFD
#define DEF_CLR_BIT2 0xFB
#define DEF_CLR_BIT3 0xF7
#define DEF_CLR_BIT4 0xEF
#define DEF_CLR_BIT5 0xDF
#define DEF_CLR_BIT6 0xBF
#define DEF_CLR_BIT7 0x7F

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define USE_MY_DEBUG 0

// ===================================================
// ��е�������������                               //
// ===================================================
#define MECHANICAL_KEYING_PIN P16D // ��е�����������

// ������ң������ֵ��
// ����������⹦�ܵĺ���ң�ذ�����ֵ
// enum
// {
//     IR_KEY_ON = 0x08,
//     IR_KEY_OFF = 0xC0,
//     IR_KEY_AUTO = 0x80,
//     IR_KEY_SPEED = 0x60,
//     IR_KEY_R3C4 = 0xA8, // R3C4��ÿ�ΰ���ʱ��ı䵱ǰ��ʾ����ɫ
//     IR_KEY_R4C4_FADE = 0xB2,
//     IR_KEY_R5C4_JUMP = 0x00,
//     IR_KEY_R6C4_ASYN_FADE = 0x58, // R6C4,ASYN_FADE
//     IR_KEY_R7C1_IN_WAVES = 0x38,  // R7C1, IN_WAVES
//     IR_KEY_R7C2_STROBE = 0x28,    // R7C2��STROBE
//     IR_KEY_R7C3_TWINKLE = 0xF0,   // R7C3��TWINKLE
//     IR_KEY_R7C4_ASYN_JUMP = 0x30, // R7C4, ASYN_JUMP

//     IR_KEY_R2C1 = 0x90,
//     IR_KEY_R2C2 = 0xB8,
//     IR_KEY_R2C3 = 0xF8,
//     IR_KEY_R2C4 = 0xB0,
//     IR_KEY_R3C1 = 0x98,
//     IR_KEY_R3C2 = 0xD8,
//     IR_KEY_R3C3 = 0x88,
//     IR_KEY_R4C1 = 0xE8,
//     IR_KEY_R4C2 = 0x48,
//     IR_KEY_R4C3 = 0x68,
//     IR_KEY_R5C1 = 0x02,
//     IR_KEY_R5C2 = 0x32,
//     IR_KEY_R5C3 = 0x20,
//     IR_KEY_R6C1 = 0x50,
//     IR_KEY_R6C2 = 0x78,
//     IR_KEY_R6C3 = 0x70,

//     /// TODO:�޸�Ϊ��ȷ�ĺ���ֵ
//     IR_KEY_4H = 0x40,
//     IR_KEY_6H = 0x12,
//     IR_KEY_8H = 0x2A,
//     IR_KEY_TIME_OFF = 0xA0,

//     IR_MECHANICAL_KEYING = 0x01,
// };

// �ͻ���ң������ֵ�����������ļ�ֵ����
// ����������⹦�ܵĺ���ң�ذ�����ֵ
enum
{
    IR_KEY_ON = 0x38,
    IR_KEY_OFF = 0xB8,
    IR_KEY_AUTO = 0x78,
    IR_KEY_SPEED = 0xF8,
    IR_KEY_R3C4 = 0xE0, // R3C4��ÿ�ΰ���ʱ��ı䵱ǰ��ʾ����ɫ
    IR_KEY_R4C4_FADE = 0xD0,
    IR_KEY_R5C4_JUMP = 0xF0,
    IR_KEY_R6C4_ASYN_FADE = 0xC8, // R6C4,ASYN_FADE
    IR_KEY_R7C1_IN_WAVES = 0x28,  // R7C1, IN_WAVES
    IR_KEY_R7C2_STROBE = 0xA8,    // R7C2��STROBE
    IR_KEY_R7C3_TWINKLE = 0x68,   // R7C3��TWINKLE
    IR_KEY_R7C4_ASYN_JUMP = 0xE8, // R7C4, ASYN_JUMP

    IR_KEY_R2C1 = 0x00,
    IR_KEY_R2C2 = 0x80,
    IR_KEY_R2C3 = 0x40,
    IR_KEY_R2C4 = 0xC0,
    IR_KEY_R3C1 = 0x20,
    IR_KEY_R3C2 = 0xA0,
    IR_KEY_R3C3 = 0x60,
    IR_KEY_R4C1 = 0x10,
    IR_KEY_R4C2 = 0x90,
    IR_KEY_R4C3 = 0x50,
    IR_KEY_R5C1 = 0x30,
    IR_KEY_R5C2 = 0xB0,
    IR_KEY_R5C3 = 0x70,
    IR_KEY_R6C1 = 0x08,
    IR_KEY_R6C2 = 0x88,
    IR_KEY_R6C3 = 0x48,

    /// TODO:�޸�Ϊ��ȷ�ĺ���ֵ
    IR_KEY_4H = 0x18,
    IR_KEY_6H = 0x98,
    IR_KEY_8H = 0x58,
    IR_KEY_TIME_OFF = 0xD8,

    IR_MECHANICAL_KEYING = 0x01,
};

// ����������⹦�ܵĺ���ң�ذ�����ֵ
// enum
// {
//     IR_KEY_ON = 0x38,
//     IR_KEY_OFF = 0xB8,
//     IR_KEY_AUTO = 0x78,
//     IR_KEY_SPEED = 0xF8,
//     IR_KEY_R3C4 = 0xE0, // R3C4��ÿ�ΰ���ʱ��ı䵱ǰ��ʾ����ɫ
//     IR_KEY_R4C4_FADE = 0xD0,
//     IR_KEY_R5C4_JUMP = 0xF0,
//     IR_KEY_R6C4_ASYN_FADE = 0xC8, // R6C4,ASYN_FADE
//     IR_KEY_R7C1_IN_WAVES = 0x28,  // R7C1, IN_WAVES
//     IR_KEY_R7C2_STROBE = 0xA8,    // R7C2��STROBE
//     IR_KEY_R7C3_TWINKLE = 0x68,   // R7C3��TWINKLE
//     IR_KEY_R7C4_ASYN_JUMP = 0xE8, // R7C4, ASYN_JUMP
// };

// �ͻ���ң������ֵ�����������ļ�ֵ��
// #define UNUSE_VAL (0xFF) // δʹ�õ�����ֵ
// const u8 table_irkey[][5] = {
//     /*
//         [][0] ����ң�صİ�����ֵ
//         [][1] ~ [][4] ������Ӧ�ģ������͵�����
//     */
//     {IR_KEY_AUTO, 0x96, 0x45, 0x0F, 0x11}, /* AUTO�������͵������ǹ̶��� */

//     {0x00, 0x00, 0x11, UNUSE_VAL, UNUSE_VAL}, /* R */
//     {0x80, 0x00, 0x22, UNUSE_VAL, UNUSE_VAL}, /* G */
//     {0x40, 0x00, 0x44, UNUSE_VAL, UNUSE_VAL}, /* B */
//     {0xC0, 0x96, 0x45, 0x10, 0x77},           /* W */

//     {0x20, 0x9F, 0x00, 0x10, 0x33}, /* R3C1 Rank 3 Column 1 �����е�һ�� */
//     {0xA0, 0x90, 0xF1, 0x10, 0x66}, /* R3C2 */
//     {0x60, 0x93, 0x0F, 0x10, 0x55}, /* R3C3 */

//     {0x10, 0x9F, 0x10, 0x10, 0x33}, /* R4C1 */
//     {0x90, 0x90, 0xF8, 0x10, 0x66}, /* R4C2 */
//     {0x50, 0x98, 0x0F, 0x10, 0x55}, /* R4C3 */

//     {0x30, 0x9F, 0x30, 0x10, 0x33}, /* R5C1 */
//     {0xB0, 0x90, 0xFC, 0x10, 0x66}, /* R5C2 */
//     {0x70, 0x9F, 0x0F, 0x10, 0x55}, /* R5C3 */

//     {0x08, 0x9F, 0xF0, 0x10, 0x33}, /* R6C1 */
//     {0x88, 0x90, 0xFF, 0x10, 0x66}, /* R6C2 */
//     {0x48, 0x9F, 0x03, 0x10, 0x55}, /* R6C3 */

//     {IR_KEY_R7C3_TWINKLE, 0x96, 0x45, 0x00, 0xEE}, /* R7C3 TWINKLE */
// };

//===============Field Protection Variables===============
u8 abuf;
u8 statusbuf;

//===============Global Function===============

//============Define  Flag=================
typedef union
{
    unsigned char byte;
    struct
    {
        u8 bit0 : 1;
        u8 bit1 : 1;
        u8 bit2 : 1;
        u8 bit3 : 1;
        u8 bit4 : 1;
        u8 bit5 : 1;
        u8 bit6 : 1;
        u8 bit7 : 1;
    } bits;
} bit_flag;
volatile bit_flag flag1;
#define flag_is_recved_data flag1.bits.bit0 // �Ƿ��յ��˺������ݣ�����û��������
#define flag_is_dev_open flag1.bits.bit1    // ��־λ���ƴ��Ƿ�ʹ��

#define last_level_in_ir_pin flag1.bits.bit2        // �ں�����ն�Ӧ���жϺ����У���ʾ�ϴ����Ŷ�Ӧ�ĵ�ƽ
#define filter_level flag1.bits.bit3                // �ں�����ն�Ӧ���жϺ����У���ʾ�˲���ĺ����źŽ������ŵĵ�ƽ
#define flag_is_recv_ir_repeat_code flag1.bits.bit4 // ��ʾ�Ƿ���յ��˺����źŵ��ظ��룬��������ң�����Ƿ��°������ɿ�

#define flag_is_press_mechanical_keying flag1.bits.bit5 // ��ʾ�Ƿ����˻�е����

// #define flag_tim_set_period_is_arrive_in_auto_mode flag1.bits.bit5 // ��ʾ�� AUTO ģʽ�£��Ƿ������ݷ��͵�����

#if USE_MY_DEBUG

#define LED_CTL_PIN P22D // ���Ƶƴ��ġ����ƴ����Ϳ������������

// // �������ϵĽ�λ��
// #define LED_CTL_PIN P04D // ���Ƶƴ��ġ����ƴ����Ϳ������������

#else

#define LED_CTL_PIN P04D // ���Ƶƴ��ġ����ƴ����Ϳ������������

#endif

// ===================================================
// ң���������������                                //
// ===================================================
#if USE_MY_DEBUG
#define IR_RECV_PIN P21D // �����źŽ�������
// // �������ϵĽ�λ��
// #define IR_RECV_PIN P16D // �����źŽ�������
#else
#define IR_RECV_PIN P15D // �����źŽ�������
#endif

volatile u8 ir_data; // ��ź�����յ�������

// ���뼶��ʱ (����1%���ڣ�1ms��10ms��100ms��ʱ������С��1%)
// ǰ��������FCPU = FHOSC / 4
void delay_ms(u16 xms)
{
    while (xms)
    {
        u16 i = 517;
        while (i--)
        {
            Nop();
        }
        xms--; // �� --��������while()�ж��������棬����ʡ�ռ�

        __asm;
        clrwdt; // ι��
        __endasm;
    }
}

// #if USE_MY_DEBUG
#define DEBUG_PIN P22D
#if 0  // ���³���Լռ��81�ֽڿռ�
// ͨ��һ�������������(����һ��Լ400ms)
// #define DEBUG_PIN P22D
void send_data_msb(u32 send_data)
{
    // �ȷ��͸�ʽͷ
    // __set_input_pull_up(); // �ߵ�ƽ
    DEBUG_PIN = 1;
    delay_ms(15);
    // __set_output_open_drain(); // �͵�ƽ
    DEBUG_PIN = 0;
    delay_ms(7); //

    for (u8 i = 0; i < 32; i++)
    {
        if ((send_data >> (32 - 1 - i)) & 0x01)
        {
            // ���Ҫ�����߼�1
            // __set_input_pull_up();  	   // �ߵ�ƽ
            DEBUG_PIN = 1;
            delay_ms(5); //
            // __set_output_open_drain(); // �͵�ƽ
            DEBUG_PIN = 0;
            delay_ms(10); //
        }
        else
        {
            // ���Ҫ�����߼�0
            // __set_input_pull_up();  	   // �ߵ�ƽ
            DEBUG_PIN = 1;
            delay_ms(5); //
            // __set_output_open_drain(); // �͵�ƽ
            DEBUG_PIN = 0;
            delay_ms(5); //
        }
    }

    // �������Ϊ�͵�ƽ
    // __set_output_open_drain(); // �͵�ƽ
    DEBUG_PIN = 0;
    delay_ms(1);
    DEBUG_PIN = 1;
    delay_ms(1);
    DEBUG_PIN = 0;
}
#endif // #if USE_MY_DEBUG

#endif

/**************************** end of file *********************************************/
