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

// const u8 table[] = {
//     0x01, // ON
//     0x04,
//     0x06,
//     0x07,
//     0x09,
//     0x0A,

//     0x0C,
//     0x0D,
//     0x0F,
//     0x10,
//     0x12,
//     0x03, // OFF
// };

volatile u8 cur_led_status; // ��¼��ǰLED��״̬

#define UNUSE_VAL (0xFF)
const u8 table[][5] = {
    /*
        [][0] ����ң�صİ�����ֵ
        [][1] ~ [][4] ������Ӧ�ģ������͵�����
    */
    {0x90, 0x00, 0x11, UNUSE_VAL, UNUSE_VAL}, /* R */
    {0xB8, 0x00, 0x22, UNUSE_VAL, UNUSE_VAL}, /* G */
    {0xF8, 0x00, 0x44, UNUSE_VAL, UNUSE_VAL}, /* B */
    {0xB0, 0x96, 0x45, 0x10, 0x77},           /* W */

    {0x98, 0x9F, 0x00, 0x10, 0x33}, /* R3C1 Rank 3 Column 1 �����е�һ�� */
    {0xD8, 0x90, 0xF1, 0x10, 0x66}, /* R3C2 */
    {0x88, 0x93, 0x0F, 0x10, 0x55}, /* R3C3 */

    {0xE8, 0x9F, 0x10, 0x10, 0x33}, /* R4C1 */ 
    {0x48, 0x90, 0xF8, 0x10, 0x66}, /* R4C2 */ 
    {0x68, 0x98, 0x0F, 0x10, 0x55}, /* R4C3 */ 


    {0x02, 0x9F, 0x30, 0x10, 0x33}, /* R5C1 */ 
    {0x32, 0x90, 0xFC, 0x10, 0x66}, /* R5C2 */ 
    {0x20, 0x9F, 0x0F, 0x10, 0x55}, /* R5C3 */ 

    {0x50, 0x9F, 0xF0, 0x10, 0x33}, /* R6C1 */ 
    {0x78, 0x90, 0xFF, 0x10, 0x66}, /* R6C2 */ 
    {0x70, 0x9F, 0x03, 0x10, 0x55}, /* R6C3 */
    
    
};

//===============Field Protection Variables===============
u8 abuf;
u8 statusbuf;

//===============Global Function===============
void Sys_Init(void);
void CLR_RAM(void);
void IO_Init(void);
void TIMER0_INT_Init(void);
void TIMER1_INT_Init(void);
void TIMER2_INT_Init(void);
void TIMER3_INT_Init(void);

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
#define flag_is_recved_data flag1.bits.bit0
#define flag_is_dev_open flag1.bits.bit1 // ��־λ���ƴ��Ƿ�ʹ��

#define last_level_in_ir_pin flag1.bits.bit2        // �ں�����ն�Ӧ���жϺ����У���ʾ�ϴ����Ŷ�Ӧ�ĵ�ƽ
#define filter_level flag1.bits.bit3                // �ں�����ն�Ӧ���жϺ����У���ʾ�˲���ĺ����źŽ������ŵĵ�ƽ
#define flag_is_recv_ir_repeat_code flag1.bits.bit4 // ��ʾ�Ƿ���յ��˺����źŵ��ظ��룬��������ң�����Ƿ��°������ɿ�

#if USE_MY_DEBUG

// #define LED_CTL_PIN P22D // ���Ƶƴ��ġ����ƴ����Ϳ������������

// �������ϵĽ�λ��
#define LED_CTL_PIN P04D // ���Ƶƴ��ġ����ƴ����Ϳ������������

#else

#define LED_CTL_PIN P04D // ���Ƶƴ��ġ����ƴ����Ϳ������������

#endif

// ===================================================
// ң���������������                                //
// ===================================================
#if USE_MY_DEBUG
// #define IR_RECV_PIN P21D // �����źŽ�������
// �������ϵĽ�λ��
#define IR_RECV_PIN P16D // �����źŽ�������
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
#if 1 // ���³���Լռ��81�ֽڿռ�
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
