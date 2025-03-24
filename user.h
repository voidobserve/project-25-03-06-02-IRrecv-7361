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

// ===================================================
// 机械按键的相关配置                               //
// ===================================================
#define MECHANICAL_KEYING_PIN P16D // 机械按键检测引脚

// 样机的遥控器键值：
// 定义具有特殊功能的红外遥控按键键值
// enum
// {
//     IR_KEY_ON = 0x08,
//     IR_KEY_OFF = 0xC0,
//     IR_KEY_AUTO = 0x80,
//     IR_KEY_SPEED = 0x60,
//     IR_KEY_R3C4 = 0xA8, // R3C4，每次按下时会改变当前显示的颜色
//     IR_KEY_R4C4_FADE = 0xB2,
//     IR_KEY_R5C4_JUMP = 0x00,
//     IR_KEY_R6C4_ASYN_FADE = 0x58, // R6C4,ASYN_FADE
//     IR_KEY_R7C1_IN_WAVES = 0x38,  // R7C1, IN_WAVES
//     IR_KEY_R7C2_STROBE = 0x28,    // R7C2，STROBE
//     IR_KEY_R7C3_TWINKLE = 0xF0,   // R7C3，TWINKLE
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

//     /// TODO:修改为正确的红外值
//     IR_KEY_4H = 0x40,
//     IR_KEY_6H = 0x12,
//     IR_KEY_8H = 0x2A,
//     IR_KEY_TIME_OFF = 0xA0,

//     IR_MECHANICAL_KEYING = 0x01,
// };

// 客户的遥控器键值（不是样机的键值）：
// 定义具有特殊功能的红外遥控按键键值
enum
{
    IR_KEY_ON = 0x38,
    IR_KEY_OFF = 0xB8,
    IR_KEY_AUTO = 0x78,
    IR_KEY_SPEED = 0xF8,
    IR_KEY_R3C4 = 0xE0, // R3C4，每次按下时会改变当前显示的颜色
    IR_KEY_R4C4_FADE = 0xD0,
    IR_KEY_R5C4_JUMP = 0xF0,
    IR_KEY_R6C4_ASYN_FADE = 0xC8, // R6C4,ASYN_FADE
    IR_KEY_R7C1_IN_WAVES = 0x28,  // R7C1, IN_WAVES
    IR_KEY_R7C2_STROBE = 0xA8,    // R7C2，STROBE
    IR_KEY_R7C3_TWINKLE = 0x68,   // R7C3，TWINKLE
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

    /// TODO:修改为正确的红外值
    IR_KEY_4H = 0x18,
    IR_KEY_6H = 0x98,
    IR_KEY_8H = 0x58,
    IR_KEY_TIME_OFF = 0xD8,

    IR_MECHANICAL_KEYING = 0x01,
};

// 定义具有特殊功能的红外遥控按键键值
// enum
// {
//     IR_KEY_ON = 0x38,
//     IR_KEY_OFF = 0xB8,
//     IR_KEY_AUTO = 0x78,
//     IR_KEY_SPEED = 0xF8,
//     IR_KEY_R3C4 = 0xE0, // R3C4，每次按下时会改变当前显示的颜色
//     IR_KEY_R4C4_FADE = 0xD0,
//     IR_KEY_R5C4_JUMP = 0xF0,
//     IR_KEY_R6C4_ASYN_FADE = 0xC8, // R6C4,ASYN_FADE
//     IR_KEY_R7C1_IN_WAVES = 0x28,  // R7C1, IN_WAVES
//     IR_KEY_R7C2_STROBE = 0xA8,    // R7C2，STROBE
//     IR_KEY_R7C3_TWINKLE = 0x68,   // R7C3，TWINKLE
//     IR_KEY_R7C4_ASYN_JUMP = 0xE8, // R7C4, ASYN_JUMP
// };

// 客户的遥控器键值（不是样机的键值）
// #define UNUSE_VAL (0xFF) // 未使用的数据值
// const u8 table_irkey[][5] = {
//     /*
//         [][0] 红外遥控的按键键值
//         [][1] ~ [][4] 按键对应的，待发送的数据
//     */
//     {IR_KEY_AUTO, 0x96, 0x45, 0x0F, 0x11}, /* AUTO，待发送的数据是固定的 */

//     {0x00, 0x00, 0x11, UNUSE_VAL, UNUSE_VAL}, /* R */
//     {0x80, 0x00, 0x22, UNUSE_VAL, UNUSE_VAL}, /* G */
//     {0x40, 0x00, 0x44, UNUSE_VAL, UNUSE_VAL}, /* B */
//     {0xC0, 0x96, 0x45, 0x10, 0x77},           /* W */

//     {0x20, 0x9F, 0x00, 0x10, 0x33}, /* R3C1 Rank 3 Column 1 第三行第一列 */
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
#define flag_is_recved_data flag1.bits.bit0 // 是否收到了红外数据，并且没有做处理
#define flag_is_dev_open flag1.bits.bit1    // 标志位，灯串是否使能

#define last_level_in_ir_pin flag1.bits.bit2        // 在红外接收对应的中断函数中，表示上次引脚对应的电平
#define filter_level flag1.bits.bit3                // 在红外接收对应的中断函数中，表示滤波后的红外信号接收引脚的电平
#define flag_is_recv_ir_repeat_code flag1.bits.bit4 // 表示是否接收到了红外信号的重复码，用于区分遥控器是否按下按键后松开

#define flag_is_press_mechanical_keying flag1.bits.bit5 // 表示是否按下了机械按键

// #define flag_tim_set_period_is_arrive_in_auto_mode flag1.bits.bit5 // 表示在 AUTO 模式下，是否到了数据发送的周期

#if USE_MY_DEBUG

#define LED_CTL_PIN P22D // 控制灯串的、给灯串发送控制命令的引脚

// // 在样板上的脚位：
// #define LED_CTL_PIN P04D // 控制灯串的、给灯串发送控制命令的引脚

#else

#define LED_CTL_PIN P04D // 控制灯串的、给灯串发送控制命令的引脚

#endif

// ===================================================
// 遥控器解码相关配置                                //
// ===================================================
#if USE_MY_DEBUG
#define IR_RECV_PIN P21D // 红外信号接收引脚
// // 在样板上的脚位：
// #define IR_RECV_PIN P16D // 红外信号接收引脚
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
#if 0  // 以下程序约占用81字节空间
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
