/******************************************************************************
;  *       @型号                 : MC32F7361
;  *       @创建日期             : 2021.12.21
;  *       @公司/作者            : SINOMCU-FAE
;  *       @晟矽微技术支持       : 2048615934
;  *       @晟矽微官网           : http://www.sinomcu.com/
;  *       @版权                 : 2021 SINOMCU公司版权所有.
;  *----------------------摘要描述---------------------------------
;  *                定时器
;  *                T0   P10 2mS翻转
;  *                T1   P11 1mS翻转
;  *                T2   P12 100uS翻转
;  *                T3   P14 50uS翻转
******************************************************************************/

#include "user.h"

// 100us延时，前提条件 ： FCPU = FHOSC / 4
void delay_100us(void)
{
    u8 i = 74;
    while (i--)
    {
        Nop();
    }
}

// 200us延时，前提条件 ： FCPU = FHOSC / 4
void delay_200us(void)
{
    u8 i = 150;
    while (i--)
    {
        Nop();
    }
}

// 1.4ms延时
void delay_1_4ms(void)
{
    u8 i;
    for (i = 0; i < 14; i++)
    {
        delay_100us();
    }
}

/************************************************
;  *    @函数名            : EEPROM_Write_Byte
;  *    @说明              : 内部EEPROM写2个字节
;  *    @输入参数          : 地址  数据
;  *    @返回参数          :
;  ***********************************************/
void EEPROM_Write_Byte(uint8_t addr, uint16_t data)
{
    HFEN = 1;
    EEAR = addr;
    EEDRH = (data >> 8) & 0xff;
    EEDRL = data & 0xff;
    GIE = 0;
    EEPR = 0x5A;
    EEPR = 0xA5; // 解除保护
    EECR = 0x01; // 启动EEPROM读写  写数据操作
    Nop();
    Nop();
    while (EEWRITE) // 检查 EE 写操作是否完成
    {
        // ClrWdt();                      //清看门狗
    }
    GIE = 1;
}
/************************************************
;  *    @函数名            : EEPROM_Read_Byte
;  *    @说明              : 内部EEPROM读2个字节
;  *    @输入参数          : 地址
;  *    @返回参数          : 读到的内容
;  ***********************************************/
uint16_t EEPROM_Read_Byte(u8 addr)
{
    uint16_t EEDR;
    EEAR = addr;
    EECR = 0x02; // 启动 EE 读操作
    Nop();
    Nop();
    while (EEREAD) // 检查 EE 写操作是否完成
    {
        // ClrWdt();                      //清看门狗
    }
    EEDR = (EEDRH << 8) | EEDRL;
    return EEDR;
}
/************************************************
;  *    @函数名            : EEPROM_Write_Data
;  *    @说明              : 连续写入
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
// void EEPROM_Write_Data(u8 addr, u16 *data, u8 len)
// {
//     while (len)
//     {
//         EEPROM_Write_Byte(addr++, *data++);
//         len--;
//     }
// }

// 发送16位的控制命令，每个16位的控制命令发送后，函数内部会延时33ms
void send_cmd_16bit(const u16 data)
{
    u8 i;
    // if (0 == flag_is_dev_open)
    // {
    //     return;
    // }

#if 1 // 控制脚按照样机的波形进行反相输出
    for (i = 0; i < 16; i++)
    {
        LED_CTL_PIN = 0;
        delay_100us();
        LED_CTL_PIN = 1;
        if ((data >> (15 - i)) & 0x01) // 如果是逻辑1
        {
            delay_ms(3);
        }
        else // 如果是逻辑0
        {
            delay_1_4ms();
        }
    }

    LED_CTL_PIN = 0;
    delay_100us();
    LED_CTL_PIN = 1;
    delay_ms(33);
#endif // 控制脚按照样机的波形进行反相输出
}

// 发送16位的控制命令，每个16位的控制命令发送后，函数内部不会延时
void send_cmd_16bit_without_delay(const u16 data)
{
    u8 i;
    // if (0 == flag_is_dev_open)
    // {
    //     return;
    // }

#if 1 // 控制脚按照样机的波形进行反相输出
    for (i = 0; i < 16; i++)
    {
        LED_CTL_PIN = 0;
        delay_100us();
        LED_CTL_PIN = 1;
        if ((data >> (15 - i)) & 0x01) // 如果是逻辑1
        {
            delay_ms(3);
        }
        else // 如果是逻辑0
        {
            delay_1_4ms();
        }
    }

    LED_CTL_PIN = 0;
    delay_100us();
    LED_CTL_PIN = 1;
#endif // 控制脚按照样机的波形进行反相输出
}

/************************************************
;  *    @函数名          : CLR_RAM
;  *    @说明            : 清RAM
;  *    @输入参数        :
;  *    @返回参数        :
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
;  *    @函数名            : IO_Init
;  *    @说明              : IO初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void IO_Init(void)
{
    IOP0 = 0x00;   // io口数据位
    OEP0 = 0x3F;   // io口方向 1:out  0:in
    PUP0 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP0 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P0ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    IOP1 = 0x00;   // io口数据位
    OEP1 = 0xFF;   // io口方向 1:out  0:in
    PUP1 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP1 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P1ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    IOP2 = 0x00; // io口数据位
    OEP2 = 0x0F; // io口方向 1:out  0:in
    PUP2 = 0x00; // io口上拉电阻   1:enable  0:disable
    PDP2 = 0x00; // io口下拉电阻   1:enable  0:disablea

    PMOD = 0x00;  // P00、P01、P13 io端口值从寄存器读，推挽输出
    DRVCR = 0x80; // 普通驱动
}

/************************************************
;  *    @函数名            : TIMER2_INT_Init
;  *    @说明              : 定时器2初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void TIMER2_INT_Init(void)
{
    T2CR = DEF_SET_BIT0 | DEF_SET_BIT1; // CPU,8分频
    T2CNT = 100 - 1;
    T2LOAD = 100 - 1; // 100us
    T2EN = 1;
    T2IE = 1;
}

void TIMER3_INT_Init(void)
{
    T3LOAD = 135 - 1;                               // 分频后，这里是1ms触发一次中断（用计算出来的值会有误差，这里加上了一些补偿）
    T3CR = (0x01 << 7) | (0x01 << 2) | (0x01 << 1); // 使能定时器，时钟源选择FCPU，64分频
    T3IE = 1;
}
/************************************************
;  *    @函数名            : Sys_Init
;  *    @说明              : 系统初始化
;  *    @输入参数          :
;  *    @返回参数          :
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
/// TODO:填充每个模式的start和end的索引
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
volatile u8 current_index;     // 当前动画索引
volatile u8 start_index;       // 当前动画开始索引
volatile u8 end_index;         // 当前动画结束索引
volatile u16 next_frame_delay; // 下次进入变色的超时值

volatile u8 power_sta;              // 开关状态
volatile u8 current_speed;          // 速度值
volatile u8 current_static;         // 当前静态模式
volatile u8 cur_mechanical_key_sta; // 机械按键对应的模式

volatile u32 timer_count = 0;

/// TODO:填充下面的所有const table数据和段的start和end值
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
    [29] = 0xEE, //   // 注意每一帧要发送10次
    [30] = 0xEE, //
    [31] = 0xEE, //
    [32] = 0xEE, //
    [33] = 0xEE, //
    [34] = 0xEE, //
    [35] = 0xEE, //
    [36] = 0xEE, //
    [37] = 0xEE, //
    [38] = 0xEE, //
    [39] = 0xAA, //   // 注意每一帧要发送10次
    [40] = 0xAA, //
    [41] = 0xAA, //
    [42] = 0xAA, //
    [43] = 0xAA, //
    [44] = 0xAA, //
    [45] = 0xAA, //
    [46] = 0xAA, //
    [47] = 0xAA, //
    [48] = 0xAA, //
    [49] = 0xCC, //  // 注意每一帧要发送10次
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
 * @param start 数据在数组中的起始位置
 * @param end  数据在数组中的结束位置
 * @param own_data1  如果总共有四帧数据要发送，这里是待发送的第二帧数据，不发送则填 0xFFFF
 * @param own_data2  如果总共有四帧数据要发送，这里是待发送的第三帧数据，不发送则填 0xFFFF
 */
void key_event_same_deal(u8 start, u8 end, u16 own_data1, u16 own_data2)
{
    if (power_sta == POWER_STA_ON)
    {
        start_index = start;
        end_index = end;

        // 保存当前效果信息
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

        current_index = start_index; // 从第一帧开始
        next_frame_delay = 0;        // 立刻发送第一帧波形
    }
    else if (power_sta == POWER_STA_OFF)
    {
        // 保存当前效果信息
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
    delay_ms(1); // 等待系统稳定

// 红外信号接收引脚：
#if USE_MY_DEBUG

    P21PU = 1;
    P21OE = 0;

    // // 在样板上的脚位：
    // P16PU = 1;
    // P16OE = 0;

#else

    // P15PU = 1; // 上拉
    P15OE = 0; // 输入模式

#endif

    // 按键检测引脚：
    // P16PU = 1; // 上拉
    P16OE = 0; // 输入模式

    // flag_is_dev_open = 1;

    // 从EEPROM中读取数据
    if (EEPROM_Read_Byte(4) != 0xA5A5)
    {
        // 保存当前效果信息
        key_event_same_deal(MODE_AUTO_START, MODE_AUTO_END, 0x9645, 0x9645);
    }
    else
    { 
        power_sta = POWER_STA_PRE_ON;
    }

    while (1)
    {
        // 机械按键扫描
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
                    current_index = start_index; // 从第一帧开始
                    next_frame_delay = 0;        // 立刻发送第一帧波形
                    send_cmd_16bit(0x0C00);
                }
                break;

            case IR_KEY_R3C4:
                current_static++;
                switch (current_static)
                {
                    /// TODO: 需要按模式数量添加case，按每个case应该显示的颜色来修改函数参数
                default:
                    current_static = 0;
                case 0:
                    // 这里直接用 main_table[] 中已有的数据 0x00EE
                    key_event_same_deal(91, 91, 0xFFFF, 0xFFFF);
                    break;
                case 1:
                    // 这里直接用 main_table[] 中已有的数据 0x00AA
                    key_event_same_deal(92, 92, 0xFFFF, 0xFFFF);
                    break;
                case 2:
                    // 这里直接用 main_table[] 中已有的数据 0x00CC
                    key_event_same_deal(93, 93, 0xFFFF, 0xFFFF);
                    break;
                }
                break;

            case IR_MECHANICAL_KEYING:
                switch (cur_mechanical_key_sta)
                {
                    /// TODO: 需要按模式数量添加case，按每个case应该显示的颜色来修改函数参数
                default:
                    cur_mechanical_key_sta = 0;
                case 0: // AUTO
                    key_event_same_deal(MODE_AUTO_START, MODE_AUTO_END, 0x9645, 0x9645);
                    break;
                case 1: // 红
                    key_event_same_deal(MODE_R2C1_START, MODE_R2C1_END, 0xFFFF, 0xFFFF);
                    break;
                case 2: // 绿
                    key_event_same_deal(MODE_R2C2_START, MODE_R2C2_END, 0xFFFF, 0xFFFF);
                    break;
                case 3: // 蓝
                    key_event_same_deal(MODE_R2C3_START, MODE_R2C3_END, 0xFFFF, 0xFFFF);
                    break;
                case 4: // 白
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
                // timer_count = 4 * 1000; // 测试用
                break;
            case IR_KEY_6H:
                timer_count = 6 * 60 * 60 * 1000;
                // timer_count = 6 * 1000; // 测试用
                break;
            case IR_KEY_8H:
                timer_count = 8 * 60 * 60 * 1000;
                // timer_count = 8 * 1000; // 测试用
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
            if (next_frame_delay == 0) // 超时了，应该发送帧数据
            {
                u16 speed_code;

                if (current_index >= SECTION_1_START && current_index <= SECTION_1_END) // 分段来获取各自的速度code和帧间隔时间
                {
                    speed_code = section_1_speed_table[current_speed];       // 获取速度code
                    next_frame_delay = section_1_delay_table[current_speed]; // 获取并设置本帧到下帧之间的延时时间
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
            // - 发送开机波形
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
            // - 发送关机波形
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
        // 每隔100us进入一次

#if 1
        { // 红外解码
            // static volatile u8 ir_fliter;
            static volatile u16 ir_level_cnt; // 红外信号的下降沿时间间隔计数
            static volatile u8 __ir_data;     //
            // static volatile u16 ir_long_press_cnt; // 檢測紅外遙控長按的計數值

            // 对每个下降沿进行计数
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
                last_level_in_ir_pin = 1; // 表示接收到的是高电平

                // 如果之前也是高电平
                if (ir_level_cnt > 1200) // 超时时间(120000us,120ms)
                {
                    // if (__ir_data != 0) // 超时，且接收到数据(红外接收处理函数中会把ir_data清零)
                    if (__ir_data != 0) // 超时，且接收到数据(现在是在中断服务函数中把__ir_data自动清零)
                    {
                        // // 带校验的版本：
                        // if ((u8)(__ir_data >> 8) == (u8)(~__ir_data)) // 如果键值的原码和反码相等
                        // {
                        // flag_is_recved_data = 1;
                        // }

                        // 不带校验的版本
                        if (0 == flag_is_recved_data)
                        {
                            ir_data = ~__ir_data;
                            __ir_data = 0;
                            flag_is_recved_data = 1;
                        }
                    }

                    flag_is_recv_ir_repeat_code = 0; // 认为遥控器按键已经按下，然后松开
                }
            }
            else
            {
                if (last_level_in_ir_pin)
                {
                    // 如果之前检测到的是高电平，现在检测到了低电平
                    if (ir_level_cnt <= 8) // 小于800us，说明接收到无效的数据，重新接收
                    {
                        // FLAG_IS_RECVED_ERR_IR_DATA = 1;
                        // flag_is_recv_ir_repeat_code = 0;
                    }
                    else if (ir_level_cnt <= 18) // 小于1800us,说明接收到了逻辑0
                    {
                        __ir_data <<= 1;

                        // P15D = 0; // 测试红外解码
                        // P15D = ~P15D; // 测试红外解码
                    }
                    else if (ir_level_cnt <= 26) // 小于2600us,说明接收到了逻辑1
                    {
                        __ir_data <<= 1;
                        __ir_data |= 0x01;

                        // P15D = 1; // 测试红外解码
                    }
                    else if (ir_level_cnt <= 150) // 小于15000us,说明接收到了格式头
                    {
                        // FLAG_IS_RECVED_ERR_IR_DATA = 1;
                        // ir_long_press_cnt = 0; // 加上这一条会检测不到长按
                    }
                    else if (ir_level_cnt <= 420) // 小于42ms,说明接收完一次完整的红外信号
                    {
#if 0 // 带校验的版本，命令源码与命令反码进行校验
    
                if ((u8)(__ir_data >> 8) == (u8)(~__ir_data)) // 如果键值的原码和反码相等
                {
                    flag_is_recved_data = 1;
                    flag_is_recv_ir_repeat_code = 1; //
                    ir_long_press_cnt = 0;
                }
#else // 不带校验的版本
                        if (0 == flag_is_recved_data)
                        {
                            ir_data = ~__ir_data;
                            __ir_data = 0;
                            flag_is_recved_data = 1;
                            flag_is_recv_ir_repeat_code = 1; //
                            // ir_long_press_cnt = 0;
                        }
#endif // 不带校验的版本
                    }
                    // else if (ir_level_cnt <= 1200) // 小于120000,120ms,说明接收到了重复码
                    // {
                    //     // if (ir_long_press_cnt < 65535)
                    //     //     ir_long_press_cnt++;
                    // }
                    else // 超过120000,说明接收到无效的数据
                    {
                    }

                    ir_level_cnt = 0;
                }

                last_level_in_ir_pin = 0; // 表示接收到的是低电平
            }
        } // 红外解码
#endif

        { // 机械按键检测
            static u16 key_press_cnt;
            if (0 == MECHANICAL_KEYING_PIN)
            {

                if (key_press_cnt == 10000) // 长按
                {
                    ir_data = IR_KEY_OFF;
                    flag_is_recved_data = 1;
                }
                else if (key_press_cnt < 10000)
                {
                    key_press_cnt++;
                }
            }
            else // 抬起
            {
                if (key_press_cnt >= 100 &&key_press_cnt < 10000) // 短按
                {
                    ir_data = IR_MECHANICAL_KEYING;
                    flag_is_recved_data = 1;
                }

                key_press_cnt = 0;
            }
        } // 机械按键检测
#if 0
        {
            static volatile u16 ir_level_cnt = 0; // 红外信号的下降沿时间间隔计数
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
                    // 下降沿

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
        // 测试通过，1ms进入一次

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
