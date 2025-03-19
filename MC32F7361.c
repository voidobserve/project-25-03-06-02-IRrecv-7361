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
void EEPROM_Write_Data(u8 addr, u16 *data, u8 len)
{
    while (len)
    {
        EEPROM_Write_Byte(addr++, *data++);
        len--;
    }
}

// 发送16位的控制命令，每个16位的控制命令发送后，函数内部会延时33ms
void send_cmd_16bit(const u16 data)
{
    u8 i;
    if (0 == flag_is_dev_open)
    {
        return;
    }

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
    if (0 == flag_is_dev_open)
    {
        return;
    }

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

/************************************************
;  *    @函数名            : main
;  *    @说明              : 主程序
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
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

    P15PU = 1; // 上拉
    P15OE = 0; // 输入模式

#endif

    // 按键检测引脚：

    flag_is_dev_open = 1;

    // 从EEPROM中读取数据

    while (1)
    {
        // 机械按键按下时就做对应的操作
        if (flag_is_recved_data)
        {
            if (ir_data == IR_KEY_ON) // ON 按键
            {

#if 1 // 控制脚按照样机的波形进行反相输出
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

                // 应该换成对应的模式的数据：

                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x08E0);
#endif // 控制脚按照样机的波形进行反相输出
            }
            else if (ir_data == IR_KEY_OFF) // OFF 按键
            {
                // send_cmd_16bit(0x0C00);
                // delay_ms(40);
                // LED_CTL_PIN = 0;
            }
            // else if (ir_data == IR_KEY_AUTO) // AUTO 按键
            // {
            //     // send_cmd_16bit(0x0C00);
            //     // send_cmd_16bit(0x9645);
            //     // send_cmd_16bit(0x9645);
            //     // send_cmd_16bit(0x0F11);
            // }
            else if (ir_data == IR_KEY_SPEED) // SPEED 按键
            {
                // 将对应的标志位置一，在对应的模式中再扫描处理
                flag_is_press_speed = 1;
            }
            else if (ir_data == IR_KEY_R3C4) // R3C4，每次按下时会改变当前显示的颜色
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

                // 将当前的状态写回EEPROM，再次上电后就进入该状态
                // 将当前R3C4的状态写回EEPROM，再次上电后就进入R3C4对应的子状态

                send_cmd_16bit(0x0C00);
                send_cmd_16bit(tmp);
            }
            else if (ir_data == IR_KEY_R4C4_FADE)
            {
                // cur_led_status = CUR_LED_STATUS_AUTO;
            }
            else
            {
                // 可以在这里查表，对程序空间进行优化
                u8 i;
                u16 tmp;

                if (IR_KEY_AUTO == ir_data) // AUTO 按键
                {
                    cur_led_status = CUR_LED_STATUS_AUTO; // 表示当前处于 AUTO 模式

                    // 清空auto模式中使用到的变量
                    // auto_mode_info = 0;
                    auto_mode_info.cur_submode_index = 0;
                    auto_mode_info.delay_time = 0;
                    auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME; // auto模式刚开始，需要等待一段时间再发送一帧数据
                }
                else
                {
                    // 如果是其他按键
                    cur_led_status = CUR_LED_STATUS_SINGLE_COLOR;
                }

                for (i = 0; i < sizeof(table_irkey) / sizeof(table_irkey[0]); i++)
                {
                    if (table_irkey[i][0] == ir_data)
                    {
                        send_cmd_16bit(0x0C00); // 几乎每个按键都要发送这个控制命令
                        tmp = ((u16)table_irkey[i][1] << 8) + (u16)table_irkey[i][2];
                        send_cmd_16bit(tmp);
                        if (table_irkey[i][3] != UNUSE_VAL && table_irkey[i][4] != UNUSE_VAL)
                        {
                            send_cmd_16bit(tmp); // 4 * 16bit的控制命令，中间的 2 * 16bit是相同的，这里只需要重复发送一次
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

        if (CUR_LED_STATUS_AUTO == cur_led_status) // 如果当前处于 AUTO 模式
        {
            // 检测是否有按下 SPEED，如果有按下，改变auto模式的子模式：
            if (flag_is_press_speed)
            {
                flag_is_press_speed = 0;

                { // 发送按下SPEED按键后，对应的数据帧
                    u8 tmp;
                    send_cmd_16bit(0x0C00); // 各个子模式下，检测到有SPEED按下，都是先发送这个
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
                } // 发送按下SPEED按键后，对应的数据帧

                auto_mode_info.cur_auto_submode++;
                if (auto_mode_info.cur_auto_submode > CUR_AUTO_SUBMODE_4)
                {
                    auto_mode_info.cur_auto_submode = CUR_AUTO_SUBMODE_1;
                }

                // 如果有按下 SPEED，切换数据发送周期，清空当前数据发送周期的计数
                // 清空auto模式使用到的计数变量
                // auto_mode_info = {0};
                // auto_mode_info.cur_auto_submode = 0;
                auto_mode_info.cur_submode_index = 0;
                // auto_mode_info.delay_time = 0;
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME; // 按下SPEED后，要重新设置一次发送时间间隔

                // 将对应的子模式写回EEPROM ========================================

                delay_ms(2); // 等待定时器清空相应的变量
            } // if (flag_is_press_speed)
            // 检测是否有按下 SPEED，如果有按下，改变auto模式的子模式

            // 设置发送时间间隔：
            if (CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME == auto_mode_info.cur_submode_status)
            {
                /*
                    一轮auto模式刚开始，数组中第0~8帧数据的延时时间，
                    子模式1是 3s
                    子模式2是10s
                    子模式3是7.55s
                    子模式4是5s
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
                        数据发送后，固定延时33ms的数据帧下标
                    */
                    auto_mode_info.delay_time = 33;
                }
                // else if ((auto_mode_info.cur_submode_index >= 10 && auto_mode_info.cur_submode_index <= 19) ||
                //          (auto_mode_info.cur_submode_index >= 32 && auto_mode_info.cur_submode_index <= 61))
                else if ((index >= 10 && index <= 19) ||
                         (index >= 32 && index <= 61))
                {
                    /*
                        数组中第 10 ~ 19 帧，第 32 ~ 61 帧
                        子模式1是 172ms
                        子模式2是 2s
                        子模式3是 1s
                        子模式4是 470ms
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
                    // 数组中第 21 ~ 26帧，28~30帧，63~72帧，固定延时5s
                    auto_mode_info.delay_time = 5000;
                }
                // else if (auto_mode_info.cur_submode_index >= 74 && auto_mode_info.cur_submode_index <= 79)
                else if (index >= 74 && index <= 79)
                {
                    /*
                        数组中第 74 ~ 79帧，
                        子模式1是 2s
                        子模式2是5.55s
                        子模式3是4.56s
                        子模式4是470ms
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
                        第81帧、最后一帧，
                        子模式1是 3s
                        子模式2是 10s
                        子模式3是 1s
                        子模式4是 5s
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

                // 设置完发送时间间隔后，进入等待模式
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_STANDBY;
            }
            // if (CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME == auto_mode_info.cur_submode_status)
            // 设置发送时间间隔

            // 如果已经准备发送数据:
            if (CUR_SUBMODE_STATUS_READY_TO_SEND == auto_mode_info.cur_submode_status)
            {
                u16 tmp = ((u16)table_auto_in_submode[auto_mode_info.cur_submode_index][0] << 8) | table_auto_in_submode[auto_mode_info.cur_submode_index][1];

                // 根据当前发送的数据帧下标，适配子模式的各个不同的数据：
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
                        // 这个模式下就是0，不用而外修改
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
                        // 这个模式下就是0，不用而外修改
                    }
                    // else if  (CUR_AUTO_SUBMODE_4 == auto_mode_info.cur_auto_submode)
                    else
                    {
                        tmp |= (0x08  << 8);
                    }
                }
                // 根据当前发送的数据帧下标，适配子模式的各个不同的数据：

                send_cmd_16bit_without_delay(tmp);
                // 发送完成后，下标加一
                auto_mode_info.cur_submode_index++;
                if (auto_mode_info.cur_submode_index >= ARRAY_SIZE(table_auto_in_submode))
                {
                    // 如果下标越界
                    auto_mode_info.cur_submode_index = 0;
                }

                // （还未确认要不要放到前面，可能要上机实际测试）这一语句要放到较前面，至少要放到发送前面，因为样机的发送时间间隔是从发送帧那一刻开始到下一帧的起始位置:
                auto_mode_info.cur_submode_status = CUR_SUBMODE_STATUS_PREPARE_TO_SET_DELAY_TIME;
            }
        }
        else // 不处于auto模式，清空auto模式使用到的变量（可以不清空）
        {
        }

    } // while (1)
}
/************************************************
;  *    @函数名            : interrupt
;  *    @说明              : 中断函数
;  *    @输入参数          :
;  *    @返回参数          :
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
        // 每隔100us进入一次
        { // 红外解码
            static volatile u8 ir_fliter;
            static volatile u16 ir_level_cnt;      // 红外信号的下降沿时间间隔计数
            static volatile u8 __ir_data;          //
            static volatile u16 ir_long_press_cnt; // 檢測紅外遙控長按的計數值

            // 对每个下降沿进行计数
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
                            ir_long_press_cnt = 0;
                        }
#endif // 不带校验的版本
                    }
                    else if (ir_level_cnt <= 1200) // 小于120000,120ms,说明接收到了重复码
                    {
                        ir_long_press_cnt++;
                    }
                    else // 超过120000,说明接收到无效的数据
                    {
                    }

                    ir_level_cnt = 0;
                }

                last_level_in_ir_pin = 0; // 表示接收到的是低电平
            }
        } // 红外解码

        { // 1ms进入一次
            static u8 cnt_100us;
            cnt_100us++;
            if (cnt_100us >= 10) // 如果计满 1ms
            {
                cnt_100us = 0;

                { // AUTO 模式下，控制发送数据的周期
                    static u16 auto_period_cnt;
                    if (cur_led_status == CUR_LED_STATUS_AUTO) // 如果当前处于 AUTO 模式
                    {
                        u8 flag_is_period_arrive; // 周期是否到来的标志位
                        auto_period_cnt++;
                        // if ((cur_auto_period == CUR_AUTO_PERIOD_3S && auto_period_cnt >= 3000) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_10S && auto_period_cnt >= 10000) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_7_5S && auto_period_cnt >= 7500) ||
                        //     (cur_auto_period == CUR_AUTO_PERIOD_5S && auto_period_cnt >= 5000))
                        // {
                        //     flag_is_period_arrive = 1;
                        // }
                        // else // 计数周期未到来：
                        // {
                        //     flag_is_period_arrive = 0;
                        // }

                        if (flag_is_period_arrive) // 如果到了发送数据的周期
                        {
                        }
                    }
                    else // 不处于 AUTO 模式
                    {
                        auto_period_cnt = 0;
                    }
                } // AUTO 模式下，控制发送数据的周期
            }

        } // 1ms进入一次
    }
    //=======T3========================
    if (T3IF & T3IE)
    {
        T3IF = 0;
        // 测试通过，1ms进入一次

        if (CUR_LED_STATUS_AUTO == cur_led_status) // 如果当前处于AUTO模式
        {
            if (CUR_SUBMODE_STATUS_STANDBY == auto_mode_info.cur_submode_status) // 如果正在等待发送时间间隔
            {
                if (auto_mode_info.delay_time > 0)
                {
                    auto_mode_info.delay_time--;
                }

                if (auto_mode_info.delay_time == 0)
                {
                    // 如果等待发送间隔的倒计时结束
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
