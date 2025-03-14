/******************************************************************************
;  *       @型号                   : MC32F7361
;  *       @创建日期               : 2021.12.21
;  *       @公司/作者              : SINOMCU-FAE
;  *       @晟矽微技术支持         : 2048615934
;  *       @晟矽微官网             : http://www.sinomcu.com/
;  *       @版权                   : 2021 SINOMCU公司版权所有.
;  *---------------------- 建议 ---------------------------------
;  *                   变量定义时使用全局变量
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

volatile u8 cur_led_status; // 记录当前LED的状态

#define UNUSE_VAL (0xFF)
const u8 table[][5] = {
    /*
        [][0] 红外遥控的按键键值
        [][1] ~ [][4] 按键对应的，待发送的数据
    */
    {0x90, 0x00, 0x11, UNUSE_VAL, UNUSE_VAL}, /* R */
    {0xB8, 0x00, 0x22, UNUSE_VAL, UNUSE_VAL}, /* G */
    {0xF8, 0x00, 0x44, UNUSE_VAL, UNUSE_VAL}, /* B */
    {0xB0, 0x96, 0x45, 0x10, 0x77},           /* W */

    {0x98, 0x9F, 0x00, 0x10, 0x33}, /* R3C1 Rank 3 Column 1 第三行第一列 */
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
#define flag_is_dev_open flag1.bits.bit1 // 标志位，灯串是否使能

#define last_level_in_ir_pin flag1.bits.bit2        // 在红外接收对应的中断函数中，表示上次引脚对应的电平
#define filter_level flag1.bits.bit3                // 在红外接收对应的中断函数中，表示滤波后的红外信号接收引脚的电平
#define flag_is_recv_ir_repeat_code flag1.bits.bit4 // 表示是否接收到了红外信号的重复码，用于区分遥控器是否按下按键后松开

#if USE_MY_DEBUG

// #define LED_CTL_PIN P22D // 控制灯串的、给灯串发送控制命令的引脚

// 在样板上的脚位：
#define LED_CTL_PIN P04D // 控制灯串的、给灯串发送控制命令的引脚

#else

#define LED_CTL_PIN P04D // 控制灯串的、给灯串发送控制命令的引脚

#endif

// ===================================================
// 遥控器解码相关配置                                //
// ===================================================
#if USE_MY_DEBUG
// #define IR_RECV_PIN P21D // 红外信号接收引脚
// 在样板上的脚位：
#define IR_RECV_PIN P16D // 红外信号接收引脚
#else
#define IR_RECV_PIN P15D // 红外信号接收引脚
#endif

volatile u8 ir_data; // 存放红外接收到的数据

// 毫秒级延时 (误差：在1%以内，1ms、10ms、100ms延时的误差均小于1%)
// 前提条件：FCPU = FHOSC / 4
void delay_ms(u16 xms)
{
    while (xms)
    {
        u16 i = 517;
        while (i--)
        {
            Nop();
        }
        xms--; // 把 --操作放在while()判断条件外面，更节省空间

        __asm;
        clrwdt; // 喂狗
        __endasm;
    }
}

// #if USE_MY_DEBUG
#define DEBUG_PIN P22D
#if 1 // 以下程序约占用81字节空间
// 通过一个引脚输出数据(发送一次约400ms)
// #define DEBUG_PIN P22D
void send_data_msb(u32 send_data)
{
    // 先发送格式头
    // __set_input_pull_up(); // 高电平
    DEBUG_PIN = 1;
    delay_ms(15);
    // __set_output_open_drain(); // 低电平
    DEBUG_PIN = 0;
    delay_ms(7); //

    for (u8 i = 0; i < 32; i++)
    {
        if ((send_data >> (32 - 1 - i)) & 0x01)
        {
            // 如果要发送逻辑1
            // __set_input_pull_up();  	   // 高电平
            DEBUG_PIN = 1;
            delay_ms(5); //
            // __set_output_open_drain(); // 低电平
            DEBUG_PIN = 0;
            delay_ms(10); //
        }
        else
        {
            // 如果要发送逻辑0
            // __set_input_pull_up();  	   // 高电平
            DEBUG_PIN = 1;
            delay_ms(5); //
            // __set_output_open_drain(); // 低电平
            DEBUG_PIN = 0;
            delay_ms(5); //
        }
    }

    // 最后，设置为低电平
    // __set_output_open_drain(); // 低电平
    DEBUG_PIN = 0;
    delay_ms(1);
    DEBUG_PIN = 1;
    delay_ms(1);
    DEBUG_PIN = 0;
}
#endif // #if USE_MY_DEBUG

#endif

/**************************** end of file *********************************************/
