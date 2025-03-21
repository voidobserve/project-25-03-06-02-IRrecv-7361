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

#define USE_MY_DEBUG 1

#define ENABLE_AUTO 0      // �Ƿ�ʹ��AUTO������Ӧ�Ĺ���(����ռ�� 801 �ֽڿռ�)
#define ENABLE_FADE 0      // �Ƿ�ʹ��FADE������Ӧ�Ĺ���(����ռ�� 348 �ֽڿռ�)
#define ENABLE_JUMP 0      // �Ƿ�ʹ��JUMP������Ӧ�Ĺ���(����ռ�� 246 �ֽڿռ�)
#define ENABLE_ASYN_FADE 0 // �Ƿ�ʹ�� ASYN_FADE ������Ӧ�Ĺ���(����ռ�� 343 �ֽڿռ�)
#define ENABLE_IN_WAVES 0  // �Ƿ�ʹ�� IN_WAVES ������Ӧ�Ĺ���(����ռ�� 339 �ֽڿռ�)
#define ENABLE_STROBE 0    // �Ƿ�ʹ�� STROBE ������Ӧ�Ĺ���(����ռ�� 319 �ֽڿռ�)
#define ENABLE_TWINKLE 0   // �Ƿ�ʹ�� TWINKLE ������Ӧ�Ĺ���(����ռ�� 255 �ֽڿռ�)
#define ENABLE_ASYN_JUMP 0 // �Ƿ�ʹ�� ASYN_JUMP ������Ӧ�Ĺ���(����ռ�� 311 �ֽڿռ�)

#define ENABLE_BUTTOM 0 // �Ƿ�ʹ�ܻ�е������Ӧ�Ĺ���
#define ENABLE_EEPROM 0 // �Ƿ�ʹ��EEPROM

#define ENABLE_4H
#define ENABLE_6H
#define ENABLE_8H
#define ENABLE_TIMER_OFF

volatile u8 flag_is_press_speed; // ��־λ���Ƿ��¹�speed��δ���д���

// ===================================================
// EEPROM���������                               //
// ===================================================

// struct
// {

// };

// ===================================================
// AUTO�������������                                //
// ===================================================
#if ENABLE_AUTO
// ����auto��ģʽ��Ҫ���͵����ݣ��ڲ��л���������˵��ÿ����ģʽ�£�������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
// �п��зָ���˵������ģʽ�µĸ����Ӳ���
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
    {0x00 | 0x00, 0x11}, // [81] �������ʾһ�ֽ���
};

// const u16 table_submode1_delay_time[] = {

// };

// ����autoģʽ�µĸ�����ģʽ
enum
{
    CUR_AUTO_SUBMODE_1 = 0, //
    CUR_AUTO_SUBMODE_2,     //
    CUR_AUTO_SUBMODE_3,     //
    CUR_AUTO_SUBMODE_4,     //
};

// volatile u8 cur_auto_submode; // ��¼autoģʽ�µ���ģʽ

// ����autoģʽ�£���ģʽ�ĸ���ִ�л���
enum
{
    CUR_SUBMODE_STATUS_NONE = 0,
    CUR_SUBMODE_STATUS_READY_TO_SEND,             // ׼����������
    CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_SUBMODE_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};

typedef struct
{
    volatile u16 delay_time; // autoģʽ�£���ǰ��ģʽ��Ӧ����ʱʱ��

    volatile u8 cur_auto_submode;   // ��¼autoģʽ�µ���ģʽ
    volatile u8 cur_submode_index;  // ��ǰִ�е���ģʽ����һ���±�
    volatile u8 cur_submode_status; // ��ǰ��ģʽ�����ڷ������ڵ��ĸ�״̬

} auto_mode_info_t;
volatile auto_mode_info_t auto_mode_info;
#endif //      #if ENABLE_AUTO

// ===================================================
// FADE�������������                                //
// ===================================================
#if ENABLE_FADE

// ����fadeģʽ�£������Է��͵�����֡
// �ڲ��л���������˵��ÿ����ģʽ�£�
// ������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
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

// ����FADEģʽ�£������׶�
enum
{
    CUR_FADE_STATUS_NONE = 0,
    CUR_FADE_STATUS_READY_TO_SEND,             // ׼����������
    CUR_FADE_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_FADE_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};

// ����FADEģʽ�£���ͬ�ķ���ʱ����
enum
{
    CUR_FADE_PERIOD_1 = 0, // 5s
    CUR_FADE_PERIOD_2,     // 2.76s
    CUR_FADE_PERIOD_3,     // 10s
    CUR_FADE_PERIOD_4,     // 7.55s
};

typedef struct
{
    volatile u16 delay_time;     // fadeģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_fade_period; // ���fadeģʽ��ǰ�ķ���ʱ����
    volatile u8 cur_fade_index;  // ��ǰִ�е����������������һ���±�
    volatile u8 cur_fade_status; // ��ǰ�����ڷ������ڵ��ĸ�״̬

} fade_info_t;
volatile fade_info_t fade_info;

#endif // #if ENABLE_FADE

#if ENABLE_JUMP

// ���� jump ģʽ�£������Է��͵�����֡
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

// ���� JUMP ģʽ�£������׶�
enum
{
    CUR_JUMP_STATUS_NONE = 0,
    CUR_JUMP_STATUS_READY_TO_SEND,             // ׼����������
    CUR_JUMP_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_JUMP_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};

// ���� JUMP ģʽ�£���ͬ�ķ���ʱ����
enum
{
    CUR_JUMP_PERIOD_1 = 0, // 170ms
    CUR_JUMP_PERIOD_2,     // 1.966s
    CUR_JUMP_PERIOD_3,     // 970ms
    CUR_JUMP_PERIOD_4,     // 470ms
};

typedef struct
{
    volatile u16 delay_time;     // jump ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_jump_period; // ��� jump ģʽ��ǰ�ķ���ʱ����
    volatile u8 cur_jump_index;  // ��ǰִ�е����������������һ���±�
    volatile u8 cur_jump_status; // ��ǰ�����ڷ������ڵ��ĸ�״̬
} jump_info_t;
volatile jump_info_t jump_info;

#endif // #if ENABLE_JUMP

#if ENABLE_ASYN_FADE

// ���� asyn_fade ģʽ�£������Է��͵�����֡
// �ڲ��л���������˵��ÿ����ģʽ�£�
// ������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
const u8 table_asyn_fade[][2] = {
    {0x00 | 0x00, 0x21},
    {0x10 | 0x00, 0x74},
    {0x00 | 0x00, 0x53},
    {0x00 | 0x00, 0x41},
    {0x00 | 0x00, 0x24},
    {0x00 | 0x00, 0x45},
};

// ���� asyn_fade ģʽ�£���ͬ����ģʽ
enum
{
    CUR_ASYN_FADE_SUBMODE_0 = 0, //  ��ģʽ0
    CUR_ASYN_FADE_SUBMODE_1,     //  ��ģʽ1
    CUR_ASYN_FADE_SUBMODE_2,     //  ��ģʽ2
    CUR_ASYN_FADE_SUBMODE_3,     //  ��ģʽ3
};

// ���� asyn_fade ģʽ�£������׶�
enum
{
    CUR_ASYN_FADE_STATUS_NONE = 0,
    CUR_ASYN_FADE_STATUS_READY_TO_SEND,             // ׼����������
    CUR_ASYN_FADE_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_ASYN_FADE_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};
typedef struct
{
    volatile u16 delay_time;           // asyn_fade ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_asyn_fade_submode; // ��� asyn_fade ģʽ��ǰ����ģʽ
    volatile u8 cur_asyn_fade_index;   // ��ǰִ�е����������������һ���±�
    volatile u8 cur_asyn_fade_status;  // ��ǰ�����ڷ������ڵ��ĸ�״̬
} asyn_fade_info_t;
volatile asyn_fade_info_t asyn_fade_info;
#endif // #if ENABLE_ASYN_FADE

#if ENABLE_IN_WAVES

// ���� in_waves ģʽ�£������Է��͵�����֡
// �ڲ��л���������˵��ÿ����ģʽ�£�
// ������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
const u8 table_in_waves[][2] = {
    {0x80 | 0x00, 0x11},
    {0x80 | 0x00, 0x12},
    {0x80 | 0x00, 0x22},
    {0x80 | 0x00, 0x24},
    {0x80 | 0x00, 0x44},
    {0x80 | 0x00, 0x14},
};

// ���� in_waves ģʽ�£���ͬ�ķ�������
enum
{
    CUR_IN_WAVES_PERIOD_0 = 0, //   5.556s
    CUR_IN_WAVES_PERIOD_1,     //   4.56s
    CUR_IN_WAVES_PERIOD_2,     //   3.36s
    CUR_IN_WAVES_PERIOD_3,     //   2s
};

// ���� in_waves ģʽ�£������׶�
enum
{
    CUR_IN_WAVES_STATUS_NONE = 0,
    CUR_IN_WAVES_STATUS_READY_TO_SEND,             // ׼����������
    CUR_IN_WAVES_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_IN_WAVES_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};
typedef struct
{
    volatile u16 delay_time;         // in_waves ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_in_waves_period; // ��� in_waves ģʽ��ǰ�ķ�������
    volatile u8 cur_in_waves_index;  // ��ǰִ�е����������������һ���±�
    volatile u8 cur_in_waves_status; // ��ǰ�����ڷ������ڵ��ĸ�״̬
} in_waves_info_t;
volatile in_waves_info_t in_waves_info;
#endif // #if ENABLE_IN_WAVES

#if ENABLE_STROBE

// ���� strobe ģʽ�£������Է��͵�����֡
// �ڲ��л���������˵��ÿ����ģʽ�£�
// ������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
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

// ���� strobe ģʽ�£���ͬ����ģʽ
enum
{
    CUR_STROBE_SUBMODE_0 = 0, //   ��ģʽ0
    CUR_STROBE_SUBMODE_1,     //   ��ģʽ1
    CUR_STROBE_SUBMODE_2,     //   ��ģʽ2
    CUR_STROBE_SUBMODE_3,     //   ��ģʽ3
};

// ���� strobe ģʽ�£������׶�
enum
{
    CUR_STROBE_STATUS_NONE = 0,
    CUR_STROBE_STATUS_READY_TO_SEND,             // ׼����������
    CUR_STROBE_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_STROBE_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};
typedef struct
{
    volatile u16 delay_time;        // strobe ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_strobe_submode; // ��� in_waves ģʽ��ǰ����ģʽ
    volatile u8 cur_strobe_index;   // ��ǰִ�е����������������һ���±�
    volatile u8 cur_strobe_status;  // ��ǰ�����ڷ������ڵ��ĸ�״̬
} strobe_info_t;
volatile strobe_info_t strobe_info;
#endif // #if ENABLE_STROBE

#if ENABLE_TWINKLE

// ���� twinkle ģʽ�£������Է��͵�����֡
const u8 table_twinkle[][2] = {
    {0x00, 0xEE},
    {0x00, 0xAA},
    {0x00, 0xCC},
};

// ���� twinkle ģʽ�£���ͬ�ķ�������
enum
{
    CUR_TWINKLE_PERIOD_0 = 0, //   �������� 1.96s
    CUR_TWINKLE_PERIOD_1,     //   �������� 964ms
    CUR_TWINKLE_PERIOD_2,     //   �������� 465ms
    CUR_TWINKLE_PERIOD_3,     //   �������� 165ms
};

// ���� twinkle ģʽ�£������׶�
enum
{
    CUR_TWINKLE_STATUS_NONE = 0,
    CUR_TWINKLE_STATUS_READY_TO_SEND,             // ׼����������
    CUR_TWINKLE_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_TWINKLE_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};
typedef struct
{
    volatile u16 delay_time;          // twinkle ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_twinkle_period;   // ��� twinkle ģʽ��ǰ�ķ�������
    volatile u8 cur_twinkle_subindex; // ��ǰִ�е����������������һ�����±꣬������ table_twinkle ���±겻ͬ����Ϊtwinkleģʽ��ÿ������֡����10�Σ��ٷ�����һ������֡
    volatile u8 cur_twinkle_status;   // ��ǰ�����ڷ������ڵ��ĸ�״̬
} twinkle_info_t;
volatile twinkle_info_t twinkle_info;
#endif // #if ENABLE_TWINKLE

#if ENABLE_ASYN_JUMP

// ���� asyn_jump ģʽ�£������Է��͵�����֡
// �ڲ��л���������˵��ÿ����ģʽ�£�
// ������в�ͬ���ڻ�����ĵڶ����������в�ͬ��
const u8 table_asyn_jump[][2] = {
    {0x00 | 0x00, 0xE0},
    {0x00 | 0x00, 0xA0},
    {0x00 | 0x00, 0xC0},
};

// ���� asyn_jump ģʽ�£���ͬ�ķ�������
enum
{
    CUR_ASYN_JUMP_SUBMODE_0 = 0, //   ��ģʽ0 -- 1000
    CUR_ASYN_JUMP_SUBMODE_1,     //   ��ģʽ1 -- 1010
    CUR_ASYN_JUMP_SUBMODE_2,     //   ��ģʽ2 -- 1001
    CUR_ASYN_JUMP_SUBMODE_3,     //   ��ģʽ3 -- 1011
};

// ���� asyn_jump ģʽ�£������׶�
enum
{
    CUR_ASYN_JUMP_STATUS_NONE = 0,
    CUR_ASYN_JUMP_STATUS_READY_TO_SEND,             // ׼����������
    CUR_ASYN_JUMP_STATUS_PREPARE_TO_SET_DELAY_TIME, // ׼�����þ��뷢����һ������֡����ʱʱ��
    CUR_ASYN_JUMP_STATUS_STANDBY,                   // ���ڵȴ���������֮֡���ʱ����
};
typedef struct
{
    volatile u16 delay_time;           // asyn_jump ģʽ�£���ǰ����ʱ������Ӧ����ʱʱ��
    volatile u8 cur_asyn_jump_submode; // ��� asyn_jump ģʽ��ǰ����ģʽ
    volatile u8 cur_asyn_jump_index;   // ��ǰִ�е����������������һ���±�
    volatile u8 cur_asyn_jump_status;  // ��ǰ�����ڷ������ڵ��ĸ�״̬
} asyn_jump_info_t;
volatile asyn_jump_info_t asyn_jump_info;
#endif // #if ENABLE_ASYN_JUMP

// ����ƹ��ģʽ:
enum
{
    CUR_LED_STATUS_OFF = 0,
    CUR_LED_STATUS_AUTO,
    CUR_LED_STATUS_SINGLE_COLOR,   // ��ɫ��ģʽ
    CUR_LED_STATUS_R3C4,           // ����R3C4��Ӧ��ģʽ��ÿ�ΰ��»�ı�LED��ǰ����ɫ
    CUR_LED_STATUS_R4C4_FADE,      // ����R4C4��Ӧ��ģʽ��FADE
    CUR_LED_STATUS_R5C4_JUMP,      // ����R5C4��Ӧ��ģʽ��JUMP
    CUR_LED_STATUS_R6C4_ASYN_FADE, // ����R6C4��Ӧ��ģʽ��ASYN_FADE
    CUR_LED_STATUS_R7C1_IN_WAVES,  // ����R7C1��Ӧ��ģʽ��IN_WAVES
    CUR_LED_STATUS_R7C2_STROBE,    // ����R7C2��Ӧ��ģʽ��STROBE
    CUR_LED_STATUS_R7C3_TWINKLE,   // ����R7C3��Ӧ��ģʽ��TWINKLE
    CUR_LED_STATUS_R7C4_ASYN_JUMP, // ����R7C4��Ӧ��ģʽ��ASYN_JUMP
};
volatile u8 cur_led_status; // ��¼��ǰLED��״̬

// ���� R3C4 ��Ӧ�Ĺ���
enum
{
    R3C4_STATUS_1 = 0,
    R3C4_STATUS_2,
    R3C4_STATUS_3,
};
volatile u8 cur_r3c4_status;

// ����������⹦�ܵĺ���ң�ذ�����ֵ
enum
{
    IR_KEY_ON = 0x08,
    IR_KEY_OFF = 0xC0,
    IR_KEY_AUTO = 0x80,
    IR_KEY_SPEED = 0x60,
    IR_KEY_R3C4 = 0xA8, // R3C4��ÿ�ΰ���ʱ��ı䵱ǰ��ʾ����ɫ
    IR_KEY_R4C4_FADE = 0xB2,
    IR_KEY_R5C4_JUMP = 0x00,
    IR_KEY_R6C4_ASYN_FADE = 0x58, // R6C4,ASYN_FADE
    IR_KEY_R7C1_IN_WAVES = 0x38,  // R7C1, IN_WAVES
    IR_KEY_R7C2_STROBE = 0x28,    // R7C2��STROBE
    IR_KEY_R7C3_TWINKLE = 0xF0,   // R7C3��TWINKLE
    IR_KEY_R7C4_ASYN_JUMP = 0x30, // R7C4, ASYN_JUMP
};

#define UNUSE_VAL (0xFF) // δʹ�õ�����ֵ
const u8 table_irkey[][5] = {
    /*
        [][0] ����ң�صİ�����ֵ
        [][1] ~ [][4] ������Ӧ�ģ������͵�����
    */
    {IR_KEY_AUTO, 0x96, 0x45, 0x0F, 0x11}, /* AUTO�������͵������ǹ̶��� */

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
#define flag_is_recved_data flag1.bits.bit0 // �Ƿ��յ��˺������ݣ�����û��������
#define flag_is_dev_open flag1.bits.bit1    // ��־λ���ƴ��Ƿ�ʹ��

#define last_level_in_ir_pin flag1.bits.bit2        // �ں�����ն�Ӧ���жϺ����У���ʾ�ϴ����Ŷ�Ӧ�ĵ�ƽ
#define filter_level flag1.bits.bit3                // �ں�����ն�Ӧ���жϺ����У���ʾ�˲���ĺ����źŽ������ŵĵ�ƽ
#define flag_is_recv_ir_repeat_code flag1.bits.bit4 // ��ʾ�Ƿ���յ��˺����źŵ��ظ��룬��������ң�����Ƿ��°������ɿ�

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
