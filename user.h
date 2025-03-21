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

#define USE_MY_DEBUG 1

#define ENABLE_AUTO 0      // 是否使能AUTO按键对应的功能(至少占用 801 字节空间)
#define ENABLE_FADE 0      // 是否使能FADE按键对应的功能(至少占用 348 字节空间)
#define ENABLE_JUMP 0      // 是否使能JUMP按键对应的功能(至少占用 246 字节空间)
#define ENABLE_ASYN_FADE 0 // 是否使能 ASYN_FADE 按键对应的功能(至少占用 343 字节空间)
#define ENABLE_IN_WAVES 0  // 是否使能 IN_WAVES 按键对应的功能(至少占用 339 字节空间)
#define ENABLE_STROBE 0    // 是否使能 STROBE 按键对应的功能(至少占用 319 字节空间)
#define ENABLE_TWINKLE 0   // 是否使能 TWINKLE 按键对应的功能(至少占用 255 字节空间)
#define ENABLE_ASYN_JUMP 0 // 是否使能 ASYN_JUMP 按键对应的功能(至少占用 311 字节空间)

#define ENABLE_BUTTOM 0 // 是否使能机械按键对应的功能
#define ENABLE_EEPROM 0 // 是否使能EEPROM

#define ENABLE_4H
#define ENABLE_6H
#define ENABLE_8H
#define ENABLE_TIMER_OFF

volatile u8 flag_is_press_speed; // 标志位，是否按下过speed且未进行处理

// ===================================================
// EEPROM的相关配置                               //
// ===================================================

// struct
// {

// };

// ===================================================
// AUTO按键的相关配置                                //
// ===================================================
#if ENABLE_AUTO
// 定义auto子模式下要发送的数据，内部有或操作运算的说明每个子模式下，这里会有不同（在或运算的第二个操作数有不同）
// 有空行分隔的说明是子模式下的各个子部份
const u8 table_auto_in_submode[][2] = {
    {(0x00 | 0x00), 0x22}, // [0]
    {(0x00 | 0x00), 0x44}, // [1]
    {(0x00 | 0x00), 0x33}, // [2]
    {(0x00 | 0x00), 0x55}, // [3]
    {(0x00 | 0x00), 0x66}, // [4]
    {(0x10 | 0x00), 0x77}, // [5]
    {(0x00 | 0x00), 0x88}, // [6]
    {(0x00 | 0x00), 0xAA}, // [7]
    {(0x00 | 0x00), 0xCC}, // [8]

    {0xC0, 0x00}, // [9]
    {0x00, 0x11}, // [10]

    {0x00, 0x22}, // [11]
    {0x00, 0x44}, // [12]
    {0x00, 0x33}, // [13]
    {0x00, 0x55}, // [14]
    {0x00, 0x66}, // [15]
    {0x10, 0x77}, // [16]
    {0x00, 0x88}, // [17]
    {0x00, 0xAA}, // [18]
    {0x00, 0xCC}, // [19]

    {0xC0, 0x00},        // [20]
    {0x00 | 0x00, 0x21}, // [21]

    {0x10 | 0x00, 0x74}, // [22]
    {0x00 | 0x00, 0x53}, // [23]
    {0x00 | 0x00, 0x41}, // [24]
    {0x00 | 0x00, 0x24}, // [25]
    {0x00 | 0x00, 0x45}, // [26]

    {0x0C, 0x00},        // [27]
    {0x00 | 0x00, 0xE0}, // [28]

    {0x00 | 0x00, 0xA0}, // [29]
    {0x00 | 0x00, 0xC0}, // [30]

    {0xC0, 0x00}, // [31]
    {0x00, 0xEE}, // [32]

    {0x00, 0xEE}, // [33]
    {0x00, 0xEE}, // [34]
    {0x00, 0xEE}, // [35]
    {0x00, 0xEE}, // [36]
    {0x00, 0xEE}, // [37]
    {0x00, 0xEE}, // [38]
    {0x00, 0xEE}, // [39]
    {0x00, 0xEE}, // [40]
    {0x00, 0xEE}, // [41]

    {0x00, 0xAA}, // [42]
    {0x00, 0xAA}, // [43]
    {0x00, 0xAA}, // [44]
    {0x00, 0xAA}, // [45]
    {0x00, 0xAA}, // [46]
    {0x00, 0xAA}, // [47]
    {0x00, 0xAA}, // [48]
    {0x00, 0xAA}, // [49]
    {0x00, 0xAA}, // [50]
    {0x00, 0xAA}, // [51]

    {0x00, 0xCC}, // [52]
    {0x00, 0xCC}, // [53]
    {0x00, 0xCC}, // [54]
    {0x00, 0xCC}, // [55]
    {0x00, 0xCC}, // [56]
    {0x00, 0xCC}, // [57]
    {0x00, 0xCC}, // [58]
    {0x00, 0xCC}, // [59]
    {0x00, 0xCC}, // [60]
    {0x00, 0xCC}, // [61]

    {0x0C, 0x00},        // [62]
    {0x00 | 0x00, 0x17}, // [63]
    {0x00 | 0x00, 0x27}, // [64]
    {0x00 | 0x00, 0x47}, // [65]
    {0x00 | 0x00, 0x37}, // [66]
    {0x00 | 0x00, 0x57}, // [67]
    {0x00 | 0x00, 0x67}, // [68]
    {0x10 | 0x00, 0x77}, // [69]
    {0x00 | 0x00, 0x87}, // [70]
    {0x00 | 0x00, 0xA7}, // [71]
    {0x00 | 0x00, 0xC7}, // [72]

    {0x0C, 0x00},        // [73]
    {0x80 | 0x00, 0x11}, // [74]

    {0x80 | 0x00, 0x12}, // [75]
    {0x80 | 0x00, 0x22}, // [76]
    {0x80 | 0x00, 0x24}, // [77]
    {0x80 | 0x00, 0x44}, // [78]
    {0x80 | 0x00, 0x14}, // [79]

    {0x0C, 0x00},        // [80]
    {0x00 | 0x00, 0x11}, // [81] 到这里表示一轮结束
};

// const u16 table_submode1_delay_time[] = {

// };

// 定义auto模式下的各个子模式
enum
{
    CUR_AUTO_SUBMODE_1 = 0, //
    CUR_AUTO_SUBMODE_2,     //
    CUR_AUTO_SUBMODE_3,     //
    CUR_AUTO_SUBMODE_4,     //
};

// volatile u8 cur_auto_submode; // 记录auto模式下的子模式

// 定义auto模式下，子模式的各个执行环节
enum
{
    CUR_SUBMODE_STATUS_NONE = 0,
    CUR_SUBMODE_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_SUBMODE_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};

typedef struct
{
    volatile u16 delay_time; // auto模式下，当前子模式对应的延时时间

    volatile u8 cur_auto_submode;   // 记录auto模式下的子模式
    volatile u8 cur_submode_index;  // 当前执行到子模式的哪一个下标
    volatile u8 cur_submode_status; // 当前子模式正处于发送周期的哪个状态

} auto_mode_info_t;
volatile auto_mode_info_t auto_mode_info;
#endif //      #if ENABLE_AUTO

// ===================================================
// FADE按键的相关配置                                //
// ===================================================
#if ENABLE_FADE

// 定义fade模式下，周期性发送的数据帧
// 内部有或操作运算的说明每个子模式下，
// 这里会有不同（在或运算的第二个操作数有不同）
const u8 table_fade[][2] = {
    {0x00 | 0x00, 0x22},
    {0x00 | 0x00, 0x44},
    {0x00 | 0x00, 0x33},
    {0x00 | 0x00, 0x55},
    {0x00 | 0x00, 0x66},
    {0x10 | 0x00, 0x77},
    {0x00 | 0x00, 0x88},
    {0x00 | 0x00, 0xAA},
    {0x00 | 0x00, 0xCC},
    {0x00 | 0x00, 0x11},
};

// 定义FADE模式下，各个阶段
enum
{
    CUR_FADE_STATUS_NONE = 0,
    CUR_FADE_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_FADE_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_FADE_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};

// 定义FADE模式下，不同的发送时间间隔
enum
{
    CUR_FADE_PERIOD_1 = 0, // 5s
    CUR_FADE_PERIOD_2,     // 2.76s
    CUR_FADE_PERIOD_3,     // 10s
    CUR_FADE_PERIOD_4,     // 7.55s
};

typedef struct
{
    volatile u16 delay_time;     // fade模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_fade_period; // 存放fade模式当前的发送时间间隔
    volatile u8 cur_fade_index;  // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_fade_status; // 当前正处于发送周期的哪个状态

} fade_info_t;
volatile fade_info_t fade_info;

#endif // #if ENABLE_FADE

#if ENABLE_JUMP

// 定义 jump 模式下，周期性发送的数据帧
const u8 table_jump[][2] = {
    {0x00, 0x22},
    {0x00, 0x44},
    {0x00, 0x33},
    {0x00, 0x55},
    {0x00, 0x66},
    {0x10, 0x77},
    {0x00, 0x88},
    {0x00, 0xAA},
    {0x00, 0xCC},
    {0x00, 0x11},
};

// 定义 JUMP 模式下，各个阶段
enum
{
    CUR_JUMP_STATUS_NONE = 0,
    CUR_JUMP_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_JUMP_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_JUMP_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};

// 定义 JUMP 模式下，不同的发送时间间隔
enum
{
    CUR_JUMP_PERIOD_1 = 0, // 170ms
    CUR_JUMP_PERIOD_2,     // 1.966s
    CUR_JUMP_PERIOD_3,     // 970ms
    CUR_JUMP_PERIOD_4,     // 470ms
};

typedef struct
{
    volatile u16 delay_time;     // jump 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_jump_period; // 存放 jump 模式当前的发送时间间隔
    volatile u8 cur_jump_index;  // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_jump_status; // 当前正处于发送周期的哪个状态
} jump_info_t;
volatile jump_info_t jump_info;

#endif // #if ENABLE_JUMP

#if ENABLE_ASYN_FADE

// 定义 asyn_fade 模式下，周期性发送的数据帧
// 内部有或操作运算的说明每个子模式下，
// 这里会有不同（在或运算的第二个操作数有不同）
const u8 table_asyn_fade[][2] = {
    {0x00 | 0x00, 0x21},
    {0x10 | 0x00, 0x74},
    {0x00 | 0x00, 0x53},
    {0x00 | 0x00, 0x41},
    {0x00 | 0x00, 0x24},
    {0x00 | 0x00, 0x45},
};

// 定义 asyn_fade 模式下，不同的子模式
enum
{
    CUR_ASYN_FADE_SUBMODE_0 = 0, //  子模式0
    CUR_ASYN_FADE_SUBMODE_1,     //  子模式1
    CUR_ASYN_FADE_SUBMODE_2,     //  子模式2
    CUR_ASYN_FADE_SUBMODE_3,     //  子模式3
};

// 定义 asyn_fade 模式下，各个阶段
enum
{
    CUR_ASYN_FADE_STATUS_NONE = 0,
    CUR_ASYN_FADE_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_ASYN_FADE_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_ASYN_FADE_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};
typedef struct
{
    volatile u16 delay_time;           // asyn_fade 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_asyn_fade_submode; // 存放 asyn_fade 模式当前的子模式
    volatile u8 cur_asyn_fade_index;   // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_asyn_fade_status;  // 当前正处于发送周期的哪个状态
} asyn_fade_info_t;
volatile asyn_fade_info_t asyn_fade_info;
#endif // #if ENABLE_ASYN_FADE

#if ENABLE_IN_WAVES

// 定义 in_waves 模式下，周期性发送的数据帧
// 内部有或操作运算的说明每个子模式下，
// 这里会有不同（在或运算的第二个操作数有不同）
const u8 table_in_waves[][2] = {
    {0x80 | 0x00, 0x11},
    {0x80 | 0x00, 0x12},
    {0x80 | 0x00, 0x22},
    {0x80 | 0x00, 0x24},
    {0x80 | 0x00, 0x44},
    {0x80 | 0x00, 0x14},
};

// 定义 in_waves 模式下，不同的发送周期
enum
{
    CUR_IN_WAVES_PERIOD_0 = 0, //   5.556s
    CUR_IN_WAVES_PERIOD_1,     //   4.56s
    CUR_IN_WAVES_PERIOD_2,     //   3.36s
    CUR_IN_WAVES_PERIOD_3,     //   2s
};

// 定义 in_waves 模式下，各个阶段
enum
{
    CUR_IN_WAVES_STATUS_NONE = 0,
    CUR_IN_WAVES_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_IN_WAVES_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_IN_WAVES_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};
typedef struct
{
    volatile u16 delay_time;         // in_waves 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_in_waves_period; // 存放 in_waves 模式当前的发送周期
    volatile u8 cur_in_waves_index;  // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_in_waves_status; // 当前正处于发送周期的哪个状态
} in_waves_info_t;
volatile in_waves_info_t in_waves_info;
#endif // #if ENABLE_IN_WAVES

#if ENABLE_STROBE

// 定义 strobe 模式下，周期性发送的数据帧
// 内部有或操作运算的说明每个子模式下，
// 这里会有不同（在或运算的第二个操作数有不同）
const u8 table_strobe[][2] = {
    {0x00 | 0x00, 0x17},
    {0x00 | 0x00, 0x27},
    {0x00 | 0x00, 0x47},
    {0x00 | 0x00, 0x37},
    {0x00 | 0x00, 0x57},
    {0x00 | 0x00, 0x67},
    {0x10 | 0x00, 0x77},
    {0x00 | 0x00, 0x87},
    {0x00 | 0x00, 0xA7},
    {0x00 | 0x00, 0xC7},
};

// 定义 strobe 模式下，不同的子模式
enum
{
    CUR_STROBE_SUBMODE_0 = 0, //   子模式0
    CUR_STROBE_SUBMODE_1,     //   子模式1
    CUR_STROBE_SUBMODE_2,     //   子模式2
    CUR_STROBE_SUBMODE_3,     //   子模式3
};

// 定义 strobe 模式下，各个阶段
enum
{
    CUR_STROBE_STATUS_NONE = 0,
    CUR_STROBE_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_STROBE_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_STROBE_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};
typedef struct
{
    volatile u16 delay_time;        // strobe 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_strobe_submode; // 存放 in_waves 模式当前的子模式
    volatile u8 cur_strobe_index;   // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_strobe_status;  // 当前正处于发送周期的哪个状态
} strobe_info_t;
volatile strobe_info_t strobe_info;
#endif // #if ENABLE_STROBE

#if ENABLE_TWINKLE

// 定义 twinkle 模式下，周期性发送的数据帧
const u8 table_twinkle[][2] = {
    {0x00, 0xEE},
    {0x00, 0xAA},
    {0x00, 0xCC},
};

// 定义 twinkle 模式下，不同的发送周期
enum
{
    CUR_TWINKLE_PERIOD_0 = 0, //   发送周期 1.96s
    CUR_TWINKLE_PERIOD_1,     //   发送周期 964ms
    CUR_TWINKLE_PERIOD_2,     //   发送周期 465ms
    CUR_TWINKLE_PERIOD_3,     //   发送周期 165ms
};

// 定义 twinkle 模式下，各个阶段
enum
{
    CUR_TWINKLE_STATUS_NONE = 0,
    CUR_TWINKLE_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_TWINKLE_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_TWINKLE_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};
typedef struct
{
    volatile u16 delay_time;          // twinkle 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_twinkle_period;   // 存放 twinkle 模式当前的发送周期
    volatile u8 cur_twinkle_subindex; // 当前执行到发送数据数组的哪一个子下标，与数组 table_twinkle 的下标不同，因为twinkle模式是每个数据帧发送10次，再发送下一种数据帧
    volatile u8 cur_twinkle_status;   // 当前正处于发送周期的哪个状态
} twinkle_info_t;
volatile twinkle_info_t twinkle_info;
#endif // #if ENABLE_TWINKLE

#if ENABLE_ASYN_JUMP

// 定义 asyn_jump 模式下，周期性发送的数据帧
// 内部有或操作运算的说明每个子模式下，
// 这里会有不同（在或运算的第二个操作数有不同）
const u8 table_asyn_jump[][2] = {
    {0x00 | 0x00, 0xE0},
    {0x00 | 0x00, 0xA0},
    {0x00 | 0x00, 0xC0},
};

// 定义 asyn_jump 模式下，不同的发送周期
enum
{
    CUR_ASYN_JUMP_SUBMODE_0 = 0, //   子模式0 -- 1000
    CUR_ASYN_JUMP_SUBMODE_1,     //   子模式1 -- 1010
    CUR_ASYN_JUMP_SUBMODE_2,     //   子模式2 -- 1001
    CUR_ASYN_JUMP_SUBMODE_3,     //   子模式3 -- 1011
};

// 定义 asyn_jump 模式下，各个阶段
enum
{
    CUR_ASYN_JUMP_STATUS_NONE = 0,
    CUR_ASYN_JUMP_STATUS_READY_TO_SEND,             // 准备发送数据
    CUR_ASYN_JUMP_STATUS_PREPARE_TO_SET_DELAY_TIME, // 准备设置距离发送下一个数据帧的延时时间
    CUR_ASYN_JUMP_STATUS_STANDBY,                   // 正在等待两个数据帧之间的时间间隔
};
typedef struct
{
    volatile u16 delay_time;           // asyn_jump 模式下，当前发送时间间隔对应的延时时间
    volatile u8 cur_asyn_jump_submode; // 存放 asyn_jump 模式当前的子模式
    volatile u8 cur_asyn_jump_index;   // 当前执行到发送数据数组的哪一个下标
    volatile u8 cur_asyn_jump_status;  // 当前正处于发送周期的哪个状态
} asyn_jump_info_t;
volatile asyn_jump_info_t asyn_jump_info;
#endif // #if ENABLE_ASYN_JUMP

// 定义灯光的模式:
enum
{
    CUR_LED_STATUS_OFF = 0,
    CUR_LED_STATUS_AUTO,
    CUR_LED_STATUS_SINGLE_COLOR,   // 单色灯模式
    CUR_LED_STATUS_R3C4,           // 按键R3C4对应的模式，每次按下会改变LED当前的颜色
    CUR_LED_STATUS_R4C4_FADE,      // 按键R4C4对应的模式，FADE
    CUR_LED_STATUS_R5C4_JUMP,      // 按键R5C4对应的模式，JUMP
    CUR_LED_STATUS_R6C4_ASYN_FADE, // 按键R6C4对应的模式，ASYN_FADE
    CUR_LED_STATUS_R7C1_IN_WAVES,  // 按键R7C1对应的模式，IN_WAVES
    CUR_LED_STATUS_R7C2_STROBE,    // 按键R7C2对应的模式，STROBE
    CUR_LED_STATUS_R7C3_TWINKLE,   // 按键R7C3对应的模式，TWINKLE
    CUR_LED_STATUS_R7C4_ASYN_JUMP, // 按键R7C4对应的模式，ASYN_JUMP
};
volatile u8 cur_led_status; // 记录当前LED的状态

// 定义 R3C4 对应的功能
enum
{
    R3C4_STATUS_1 = 0,
    R3C4_STATUS_2,
    R3C4_STATUS_3,
};
volatile u8 cur_r3c4_status;

// 定义具有特殊功能的红外遥控按键键值
enum
{
    IR_KEY_ON = 0x08,
    IR_KEY_OFF = 0xC0,
    IR_KEY_AUTO = 0x80,
    IR_KEY_SPEED = 0x60,
    IR_KEY_R3C4 = 0xA8, // R3C4，每次按下时会改变当前显示的颜色
    IR_KEY_R4C4_FADE = 0xB2,
    IR_KEY_R5C4_JUMP = 0x00,
    IR_KEY_R6C4_ASYN_FADE = 0x58, // R6C4,ASYN_FADE
    IR_KEY_R7C1_IN_WAVES = 0x38,  // R7C1, IN_WAVES
    IR_KEY_R7C2_STROBE = 0x28,    // R7C2，STROBE
    IR_KEY_R7C3_TWINKLE = 0xF0,   // R7C3，TWINKLE
    IR_KEY_R7C4_ASYN_JUMP = 0x30, // R7C4, ASYN_JUMP
};

#define UNUSE_VAL (0xFF) // 未使用的数据值
const u8 table_irkey[][5] = {
    /*
        [][0] 红外遥控的按键键值
        [][1] ~ [][4] 按键对应的，待发送的数据
    */
    {IR_KEY_AUTO, 0x96, 0x45, 0x0F, 0x11}, /* AUTO，待发送的数据是固定的 */

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

    {IR_KEY_R7C3_TWINKLE, 0x96, 0x45, 0x00, 0xEE}, /* R7C3 TWINKLE */
};

//===============Field Protection Variables===============
u8 abuf;
u8 statusbuf;

//===============Global Function===============
// void Sys_Init(void);
// void CLR_RAM(void);
// void IO_Init(void);
// void TIMER0_INT_Init(void);
// void TIMER1_INT_Init(void);
// void TIMER2_INT_Init(void);
// void TIMER3_INT_Init(void);

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
