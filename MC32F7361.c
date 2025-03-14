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
    // TIMER3_INT_Init();
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

// 红外信号接收引脚：
#if USE_MY_DEBUG

    // P21PU = 1;
    // P21OE = 0;

    // 在样板上的脚位：
    P16PU = 1;
    P16OE = 0;

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
            if (ir_data == 0x08) // ON 按键
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

                // 应该换成对应的模式：

                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x08E0);
#endif // 控制脚按照样机的波形进行反相输出
            }
            else if (ir_data == 0xC0) // OFF 按键
            {
                // send_cmd_16bit(0x0C00);
                // delay_ms(40);
                // LED_CTL_PIN = 0;
            }
            else if (ir_data == 0x80) // AUTO 按键
            {
                // send_cmd_16bit(0x0C00);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x9645);
                // send_cmd_16bit(0x0F11);
            }
            else if (ir_data == 0x60) // SPEED 按键
            {
            }
            else if (0)  // R3C4
            {

            }
            else if (0)
            {
                
            }
            else
            {
                // 可以在这里查表，对程序空间进行优化
                u8 i;
                u16 tmp;
                for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
                {
                    if (table[i][0] == ir_data)
                    {
                        send_cmd_16bit(0x0C00); // 几乎每个按键都要发送这个控制命令
                        tmp = ((u16)table[i][1] << 8) + (u16)table[i][2];
                        send_cmd_16bit(tmp);
                        if (table[i][3] != UNUSE_VAL && table[i][4] != UNUSE_VAL)
                        {
                            send_cmd_16bit(tmp); // 4 * 16bit的控制命令，中间的 2 * 16bit是相同的，这里只需要重复发送一次
                            tmp = ((u16)table[i][3] << 8) + (u16)table[i][4];
                            send_cmd_16bit(tmp);
                        }
                        break;
                    }
                }
            }

            ir_data = 0;
            flag_is_recved_data = 0;
        }
    }
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
    if (T0IF & T0IE)
    {
        P10D = !P10D;
        T0IF = 0;
    }
    //=======T1========================
    if (T1IF & T1IE)
    {
        P11D = !P11D;
        T1IF = 0;
    }
    //=======T2========================
    if (T2IF & T2IE)
    {
        T2IF = 0;

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
    }
    //=======T3========================
    if (T3IF & T3IE)
    {
        T3IF = 0;
    }
    __asm;
    swapar _statusbuf;
    movra _PFLAG;
    swapr _abuf;
    swapar _abuf;
    __endasm;
}

/**************************** end of file *********************************************/
