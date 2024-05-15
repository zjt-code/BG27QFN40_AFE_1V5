/*
 * afe.c
 *
 *  Created on: 2024年5月13日
 *      Author: SZZ0030
 */

#include "em_gpio.h"

#include "afe.h"

//#include "bms3_driver.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "gpiointerrupt.h"
#include "app_log.h"
#include "sl_udelay.h"
#include "sl_bt_api.h"
//#include "sl_iostream_usart_vcom_config.h"
//#include "sl_iostream_init_instances.h"
#include "sl_spidrv_inst0_config.h"
#include "sl_spidrv_instances.h"







void AFE_Init_Gpio()
{

  GPIO_PinModeSet(BSP_GPIO_CHIP_ENA_PORT,BSP_GPIO_CHIP_ENA_PIN,gpioModePushPull,0);
  GPIO_PinModeSet(BSP_GPIO_CHIP_WAKEUP_ENA_PORT,BSP_GPIO_CHIP_WAKEUP_ENA_PIN,gpioModePushPull,0);

}


/*
 * bms3_driver.c
 *
 *  Created on: 2023年7月19日
 *      Author: xion_huang
 */




// BMS3 日志开关
#if 1
#define BMS3_Driver_log_info(...)  app_log_info(__VA_ARGS__)
#else
#define BMS3_Driver_log_info(...)
#endif

#if 0 // BRD4110A  spi clk pb2 rx pb1 tx pb0
// BMS3中断脚配置
#define BMS3_GPIOINT_PORT gpioPortC
#define BMS3_GPIOINT_PIN 4
/* 0: pins 0-3   (interrupt number 0-3)
 * 1: pins 4-7   (interrupt number 4-7)
 * 2: pins 8-11  (interrupt number 8-11)
 * 3: pins 12-15 (interrupt number 12-15)
 */
#define BMS3_GPIOINT_INTNUM 4

// BMS3 EN脚配置
#define BMS3_EN_PORT gpioPortC
#define BMS3_EN_PIN 0
#define BMS3_ENABLE GPIO_PinOutSet(BMS3_EN_PORT, BMS3_EN_PIN)
#define BMS3_DISABLE GPIO_PinOutClear(BMS3_EN_PORT, BMS3_EN_PIN)

// BMS3 WAKEUP脚配置
#define BMS3_WAKEUP_PORT gpioPortC
#define BMS3_WAKEUP_PIN 1
#define BMS3_WAKEUP_H GPIO_PinOutSet(BMS3_WAKEUP_PORT, BMS3_WAKEUP_PIN)
#define BMS3_WAKEUP_L GPIO_PinOutClear(BMS3_WAKEUP_PORT, BMS3_WAKEUP_PIN)

// SPI CS脚配置
#define BMS3_SPI_CS_PORT gpioPortC
#define BMS3_SPI_CS_PIN 6
#define BMS3_SPI_CS_PIN_H GPIO_PinOutSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN)
#define BMS3_SPI_CS_PIN_L GPIO_PinOutClear(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN)

#else // custom board  spi clk pc5 rx pc2 tx pc1
// BMS3中断脚配置
#define BMS3_GPIOINT_PORT gpioPortA
#define BMS3_GPIOINT_PIN          4
/* 0: pins 0-3   (interrupt number 0-3)
 * 1: pins 4-7   (interrupt number 4-7)
 * 2: pins 8-11  (interrupt number 8-11)
 * 3: pins 12-15 (interrupt number 12-15)
 */
#define BMS3_GPIOINT_INTNUM         4

// BMS3 EN脚配置
#define BMS3_EN_PORT  gpioPortB
#define BMS3_EN_PIN   1
#define BMS3_ENABLE   GPIO_PinOutSet(BMS3_EN_PORT, BMS3_EN_PIN)
#define BMS3_DISABLE  GPIO_PinOutClear(BMS3_EN_PORT, BMS3_EN_PIN)

// BMS3 WAKEUP脚配置
#define BMS3_WAKEUP_PORT gpioPortB
#define BMS3_WAKEUP_PIN 0
#define BMS3_WAKEUP_H GPIO_PinOutSet(BMS3_WAKEUP_PORT, BMS3_WAKEUP_PIN)
#define BMS3_WAKEUP_L GPIO_PinOutClear(BMS3_WAKEUP_PORT, BMS3_WAKEUP_PIN)

// SPI CS脚配置
#define BMS3_SPI_CS_PORT gpioPortA
#define BMS3_SPI_CS_PIN 3
#define BMS3_SPI_CS_PIN_H GPIO_PinOutSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN)
#define BMS3_SPI_CS_PIN_L GPIO_PinOutClear(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN)

#endif

// 反馈电阻选择
uint8_t CH1_WE1_RFB_SEL = 0x39;
// 增益
uint8_t CH1_WE1_VGAIN_SEL = 0x0;
// 偏置
//uint8_t CH1_DINWE_L8 =0x43; // WE1DAC偏置电压设置低8位
//uint8_t CH1_DINWE_H2 =0x1; // WE1DAC偏置电压设置高2位

uint8_t CH1_DINWE_L8 =0x96; // WE1DAC偏置电压设置低8位
uint8_t CH1_DINWE_H2 =0x0; // WE1DAC偏置电压设置高2位
//// 修改ICLK&PCLK&CIC
//uint8_t CLK = 0x3E;
uint8_t CLK = 0x26;
uint8_t CIC = 0x61;

void BMS3_Driver_Int_cb(uint8_t intNo)
{
   BMS3_Driver_log_info("BMS3_Driver_Int_cb\n");
  if (BMS3_GPIOINT_INTNUM == intNo)
  {
    if (GPIO_PinInGet(BMS3_GPIOINT_PORT, BMS3_GPIOINT_PIN))
    {
//      BMS3_Driver_log_info("sl_bt_external_signal\n");
        sl_bt_external_signal(BMS3_DRIVER_INT_EVT);
    }
  }
}


void BMS3_Driver_Init(void)
{
  static uint8_t first_init_flag = 1;
  // SPI已经在添加组件后，由系统初始化，此处不需初始化SPI

  if (first_init_flag) // 首次初始化
  {
    BMS3_Driver_log_info("First Init\n");

    // 使能GPIO时钟
    CMU_ClockEnable(cmuClock_GPIO, true);

    // 初始化SPI的CS脚
    GPIO_PinModeSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN, gpioModePushPull, 1);

    // 初始化BMS3 EN脚
    GPIO_PinModeSet(BMS3_EN_PORT, BMS3_EN_PIN, gpioModePushPull, 1);
    // 初始化BMS3 WAKEUP脚
    GPIO_PinModeSet(BMS3_WAKEUP_PORT, BMS3_WAKEUP_PIN, gpioModePushPull, 1);

    // 设置GPIO中断
    GPIO_PinModeSet(BMS3_GPIOINT_PORT, BMS3_GPIOINT_PIN, gpioModeInputPull, 1);
    GPIO_ExtIntConfig(BMS3_GPIOINT_PORT,
                      BMS3_GPIOINT_PIN,
                      BMS3_GPIOINT_INTNUM,
                      true,
                      false,
                      true);
    // 使能中断
//    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
//    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    GPIOINT_CallbackRegister(BMS3_GPIOINT_INTNUM, BMS3_Driver_Int_cb);

    // 拉高BMS3 EN和WAKEUP，EN脚需要一直保持高电平，否则AFE的偏置电压会丢失，导致唤醒后测量不准
    BMS3_ENABLE;
    BMS3_WAKEUP_H;

    sl_sleeptimer_delay_millisecond(1000); // 延时等待BMS3启动

    // 初始化配置BMS3
    BMS3_Driver_CFG_BURST_ALL();
//    BMS3_Driver_CFG_BURST_ALL_SECOND();

    // 测试
//    BMS3_DISABLE;
//    BMS3_WAKEUP_L;
//    BMS3_Driver_log_info("BMS3 OFF\n");
//    GPIO_PinModeSet(SL_SPIDRV_USART_INST_TX_PORT, SL_SPIDRV_USART_INST_TX_PIN, gpioModeDisabled, 0);
//    GPIO_PinModeSet(SL_SPIDRV_USART_INST_RX_PORT, SL_SPIDRV_USART_INST_RX_PIN, gpioModeDisabled, 0);
//    GPIO_PinModeSet(SL_SPIDRV_USART_INST_CLK_PORT, SL_SPIDRV_USART_INST_CLK_PIN, gpioModeDisabled, 0);
//    GPIO_PinModeSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN, gpioModeDisabled, 0);
   // if(count++ > 2)
      first_init_flag = 0;
  }
  else
  {
//    sl_iostream_init_instances();
//    sl_spidrv_init_instances();
//    GPIO_PinModeSet(SL_IOSTREAM_USART_VCOM_TX_PORT, SL_IOSTREAM_USART_VCOM_TX_PIN, gpioModePushPull, 1);
//    GPIO_PinModeSet(SL_IOSTREAM_USART_VCOM_RX_PORT, SL_IOSTREAM_USART_VCOM_RX_PIN, gpioModeInputPull, 1);
    BMS3_WAKEUP_H;
    sl_sleeptimer_delay_millisecond(2); // 延时等待BMS唤醒

    BMS3_Driver_log_info("BMS3 Init\n");

    GPIO_ExtIntConfig(BMS3_GPIOINT_PORT,
                      BMS3_GPIOINT_PIN,
                      BMS3_GPIOINT_INTNUM,
                      true,
                      false,
                      true);
    // 使能中断
//    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
//    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    GPIOINT_CallbackRegister(BMS3_GPIOINT_INTNUM, (GPIOINT_IrqCallbackPtr_t)BMS3_Driver_Int_cb);

    // 初始化SPI的CS脚
    GPIO_PinModeSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN, gpioModePushPull, 1);

    BMS3_Driver_CFG_BURST_ALL_SECOND();
  }
}

void BMS3_Driver_Deinit(void)
{
  GPIO_PinModeSet(BMS3_SPI_CS_PORT, BMS3_SPI_CS_PIN, gpioModeInput, 0);

  // BMS进入休眠
  BMS3_WAKEUP_L;
}

#if 0 // 正常采样版本
float BMS3_Driver_EXTI_IMEAS_INT_CFG(void)
{
    static uint16_t i =0;
    static uint16_t a =0;
    static uint16_t buff_we1 = 0;
    uint16_t buff1 = 0;     //地址值
    uint16_t buff2 = 0;     //AD值
    uint16_t buff3 = 0;     //AD值
    int buff = 0;      //AD中转存储值
    float ad = 0.0;    //AD转换后的电流测量值?
    BMS3_SPI_CS_PIN_H;

    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    //////8  addr 0x05
    USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
    USART_SpiTransfer(USART0, WR_SINGLE_CMD);
    USART_SpiTransfer(USART0, 0x1);
    USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //////9   0x7 数据低8位
    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_CH0DATA_0);
    USART_SpiTransfer(USART0, RD_SINGLE_CMD);
    USART_SpiTransfer(USART0, PAD);
    buff2 = USART_SpiTransfer(USART0, PAD);
    buff = buff2;
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //////9   0x8 数据高8位
    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_CH0DATA_1);
    USART_SpiTransfer(USART0, RD_SINGLE_CMD);
    USART_SpiTransfer(USART0, PAD);
    buff2 = USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;
    buff = (buff2<<8)+buff;
//    BMS3_Driver_log_info("buff : %d\n", buff);
    i++;
    if(i<14)
    {
      // BMS3_Driver_log_info("*\n");
    }
    if(i == 14)
    {
        buff_we1 = buff;
        sl_udelay_wait(2);
        BMS3_SPI_CS_PIN_L;
        USART_SpiTransfer(USART0, 0x3A);
        USART_SpiTransfer(USART0, WR_SINGLE_CMD);
        USART_SpiTransfer(USART0, CLK);
        USART_SpiTransfer(USART0, PAD);
        sl_udelay_wait(30);
        BMS3_SPI_CS_PIN_H;

        //0x3B写B & 0x3A[0]置0
        sl_udelay_wait(2);
        BMS3_SPI_CS_PIN_L;
        USART_SpiTransfer(USART0, 0x3B);
        USART_SpiTransfer(USART0, WR_SINGLE_CMD);
        USART_SpiTransfer(USART0, 0xb);
        USART_SpiTransfer(USART0, PAD);
        sl_udelay_wait(30);
        sl_udelay_wait(1);
        BMS3_SPI_CS_PIN_H;
        //ELE_BUF使能+模式    we1 f工作电极1之电压   we1out d 工作电极1之运放输出电压     通道:101：缓冲器（ELE_BUFFER_OUT）之输出
        sl_udelay_wait(2);
        BMS3_SPI_CS_PIN_L;
        USART_SpiTransfer(USART0, 0x61);
        USART_SpiTransfer(USART0, WR_SINGLE_CMD);
        USART_SpiTransfer(USART0, 0xd);
        USART_SpiTransfer(USART0, PAD);
        sl_udelay_wait(30);
        sl_udelay_wait(1);
        BMS3_SPI_CS_PIN_H;

        sl_udelay_wait(2);
        BMS3_SPI_CS_PIN_L;
        USART_SpiTransfer(USART0, 0x3A);
        USART_SpiTransfer(USART0, WR_SINGLE_CMD);
        USART_SpiTransfer(USART0, CLK|0x80);
        USART_SpiTransfer(USART0, PAD);
        sl_udelay_wait(30);
        BMS3_SPI_CS_PIN_H;

        sl_udelay_wait(5);
        BMS3_SPI_CS_PIN_L;
        USART_SpiTransfer(USART0, 0x61);
        USART_SpiTransfer(USART0, RD_SINGLE_CMD);
        buff3 = USART_SpiTransfer(USART0, PAD);
        BMS3_Driver_log_info("addr:%x\n",buff3);
        buff3 = USART_SpiTransfer(USART0, PAD);
        BMS3_Driver_log_info("0x61:%x\n",buff3);
        sl_udelay_wait(1);
        BMS3_SPI_CS_PIN_H;

    }
    if(i>=15)
    {
      i=15;
//    ad =(buff - buff_we1)/32768.0*1.177*1000 ;                            //CSP 3号
//    ad =(buff - buff_we1)/32768.0*1.206*1000 ;                            //CSP 2号
//    ad =(buff - buff_we1)/32768.0*1.222*1000 ;                            //CSP 1号
//    ad =(buff - buff_we1)/32768.0*1.205*1000 ;                            //QFN 3V 测试版1
//    ad =(buff - buff_we1)/32768.0*1.257*1000 ;                            //QFN 1.5v 2
//    ad =(buff - buff_we1)/32768.0*1.26*1000/10;                           //QFN 1.5v 2
//    ad =(buff - buff_we1)/32768.0*1.26/10000000*1000000000;               //QFN 1.5v 2  //AD电压差/10M*10^9 = 钠安

//    ad =(buff - buff_we1)/32768.0*1.26*100;                           //QFN 1.5v 2    na                标准差1号
//    ad =(buff - buff_we1)/32768.0*1.129*100;                          //QFN 1.5v 测标准差   na           标准差2号
//    ad =(buff - buff_we1)/32768.0*1.149*100;                          //QFN 1.5v 测标准差   na           标准差3号

//      ad =(buff - buff_we1)/32768.0*1.222*100*72.5/76.37;                           //QFN 1.5v 测标准差   na           标准差4号
//      ad =(buff - buff_we1)/32768.0*1.179*1000*70/76;                           //QFN 1.5v 测标准差   na           标准差4号
//      ad =(buff - buff_we1)/32768.0*1.207*1000*29.3/28.8;                           //0.293v

//      ad =(buff - buff_we1)/32768.0*1.058*1000*25.8/27;                           //0.293v
      ad =(buff - buff_we1)/32768.0*1.058*100*25.8/27;
      BMS3_Driver_log_info("%.5f\n",ad);
    }
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //addr:0x4  清除中断状态
    sl_udelay_wait(1000);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_INT);
    USART_SpiTransfer(USART0, WR_SINGLE_CMD);
    USART_SpiTransfer(USART0, 0x1);
    USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(5);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    return ad;
}
#else // 定时采样版本
float BMS3_Driver_EXTI_IMEAS_INT_CFG(void)
{
    static uint16_t i =0;
    static uint16_t i_i =0;
    static uint16_t a =0;
    static uint16_t buff_we1 = 0;
    uint16_t buff1 = 0;     //地址值
    uint16_t buff2 = 0;     //AD值
    uint16_t buff3 = 0;     //AD值
    int buff = 0;      //AD中转存储值
    float ad = 0.0;    //AD转换后的电流测量值?
    BMS3_SPI_CS_PIN_H;

    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    //////8  addr 0x05
    USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
    USART_SpiTransfer(USART0, WR_SINGLE_CMD);
    USART_SpiTransfer(USART0, 0x1);
    USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //////9   0x7 数据低8位
    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_CH0DATA_0);
    USART_SpiTransfer(USART0, RD_SINGLE_CMD);
    USART_SpiTransfer(USART0, PAD);
    buff2 = USART_SpiTransfer(USART0, PAD);
    buff = buff2;
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //////9   0x8 数据高8位
    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_CH0DATA_1);
    USART_SpiTransfer(USART0, RD_SINGLE_CMD);
    USART_SpiTransfer(USART0, PAD);
    buff2 = USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;
    buff = (buff2<<8)+buff;
//    BMS3_Driver_log_info("buff : %d\n", buff);
    i++;

    if(i == 14)
    {
        buff_we1 = buff;
    }
    if(i >= 15)
    {
      i = 15;
      i_i++;
      if(i_i >= 3)
      {
        i_i = 3;
//        ad =(buff - buff_we1)/32768.0*1.268*100*75.4/79.5;                            //QFN  标准1  0.757
         ad =(buff - buff_we1)/32768.0*1.268*100*55.0f/64.0f;                            //QFN  标准1  0.757
//        ad =(buff - buff_we1)/32768.0*1.058*100*25.8/27;
        BMS3_Driver_log_info("%.5f\n",ad);

        GPIO_ExtIntConfig(BMS3_GPIOINT_PORT,
                            BMS3_GPIOINT_PIN,
                            BMS3_GPIOINT_INTNUM,
                            false,
                            false,
                            false);
      }
    }
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    //addr:0x4  清除中断状态
    sl_udelay_wait(2);
    BMS3_SPI_CS_PIN_L;
    USART_SpiTransfer(USART0, IMEAS_INT);
    USART_SpiTransfer(USART0, WR_SINGLE_CMD);
    USART_SpiTransfer(USART0, 0x1);
    USART_SpiTransfer(USART0, PAD);
    sl_udelay_wait(5);
    sl_udelay_wait(1);
    BMS3_SPI_CS_PIN_H;

    if((i == 15) && (i_i == 3))
    {
      i_i = 0;
      // CFG_BURST_KEEP_ANA();

      sl_udelay_wait(3);
      BMS3_SPI_CS_PIN_L;
      sl_udelay_wait(15);
      USART_SpiTransfer(USART0, 0x3A);
      USART_SpiTransfer(USART0, WR_BURST_CMD);
      USART_SpiTransfer(USART0, CLK);   //3Ah
      USART_SpiTransfer(USART0, PAD);
      sl_udelay_wait(30);
      BMS3_SPI_CS_PIN_H;

      //使能BG,DAC  关闭BG
      sl_udelay_wait(2);
      BMS3_SPI_CS_PIN_L;
      USART_SpiTransfer(USART0, 0x50);
      USART_SpiTransfer(USART0, WR_SINGLE_CMD);
//      USART_SpiTransfer(USART0, 0x02);
      USART_SpiTransfer(USART0, 0x03);
      USART_SpiTransfer(USART0, PAD);
      sl_udelay_wait(30);
      BMS3_SPI_CS_PIN_H;

      sl_udelay_wait(2);
      BMS3_SPI_CS_PIN_L;
      USART_SpiTransfer(USART0, 0x3A);
      USART_SpiTransfer(USART0, WR_SINGLE_CMD);
      USART_SpiTransfer(USART0, CLK|0x80);
      USART_SpiTransfer(USART0, PAD);
      sl_udelay_wait(30);
      BMS3_SPI_CS_PIN_H;

      // 采集结束
     BMS3_WAKEUP_L;

      // 清除GPIO中断，计算时间比较长，期间BMS3可能会产生一个IO中断
      GPIO_IntClear(1 << BMS3_GPIOINT_INTNUM);
//      while(!(USART_StatusGet(USART0) & (1 << 13)));  // 等待SPI数据
//      USART_Reset(USART0);
//      GPIO_PinModeSet(SL_SPIDRV_USART_INST_TX_PORT, SL_SPIDRV_USART_INST_TX_PIN, gpioModeDisabled, 0);
//      GPIO_PinModeSet(SL_SPIDRV_USART_INST_RX_PORT, SL_SPIDRV_USART_INST_RX_PIN, gpioModeDisabled, 0);
//      GPIO_PinModeSet(SL_SPIDRV_USART_INST_CLK_PORT, SL_SPIDRV_USART_INST_CLK_PIN, gpioModeDisabled, 0);

//      while(!(USART_StatusGet(USART1) & (1 << 13)));  // 等待串口数据发完
//      USART_Reset(USART1);
//      GPIO_PinModeSet(SL_IOSTREAM_USART_VCOM_TX_PORT, SL_IOSTREAM_USART_VCOM_TX_PIN, gpioModeDisabled, 0);
//      GPIO_PinModeSet(SL_IOSTREAM_USART_VCOM_RX_PORT, SL_IOSTREAM_USART_VCOM_RX_PIN, gpioModeDisabled, 0);
    }

    return ad;
}
#endif

void BMS3_Driver_CFG_ANA(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;

  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  //16   1E    26   2E   36    3E
  //32K  16K   8k   4K   2K    1K
  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CLK);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3B);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xb);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  //使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x03);
//  USART_SpiTransfer(USART0, 0x02);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  //修改反馈电阻
  //3     7     B     F     13    17
  //50K   100K  200K  350K  5K  750K
  //1B    1F    23    27    2B    2f    33    37   3B
  //1M    2M    3M    4M    5M    6M    7M    8M   10M
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //参比电极以及辅助电极使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x57);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //工作电极的偏置电压由第一个DAC生成使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x58);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //设置偏置电压[0:7]+[8:9]
  //0   0.3   3f  0.4  7f  0.511  ff  0.724
  //3f+1  0.83V   8f+1  0.963   df+1  1.096   ff+1  1.149
  //f+2   1.176   3f+2  1.256   7f+2  1.363   ff+2  1.489   ff+3  1.496
  //工作电极的偏置电压第一个DAC,电压控制信号
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_DINWE_L8);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_DINWE_H2);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //参比电极的偏置电压由第二个DAC生成使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5b);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //配置CE[0:7]+[8:9]
  //参比电极的偏置电压由第二个DAC,，电压控制信号
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5C);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x7f);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //控制ADC使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x62);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //DC CLK  //模拟保持分频&关闭BG
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3C);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x7);   //锟斤拷频系锟斤拷
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xBE);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  BMS3_Driver_log_info("CFG_ANA FINISH!\r\n");
}


void BMS3_Driver_CFG_KEEP_ANA(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;

  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  //16   1E    26   2E   36    3E
  //32K  16K   8k   4K   2K    1K
  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CLK);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3B);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xb);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  //配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  //修改反馈电阻
  //3     7     B     F     13    17
  //50K   100K  200K  350K  500K  750K
  //1B    1F    23    27    2B    2f    33    37   3B
  //1M    2M    3M    4M    5M    6M    7M    8M   10M
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //参比电极以及辅助电极使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x57);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //工作电极的偏置电压由第一个DAC生成使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x58);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //参比电极的偏置电压由第二个DAC生成使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5b);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //设置偏置电压[0:7]+[8:9]
  //0   0.3   3f  0.4  7f  0.511  ff  0.724
  //3f+1  0.83V   8f+1  0.963   df+1  1.096   ff+1  1.149
  //f+2   1.176   3f+2  1.256   7f+2  1.363   ff+2  1.489   ff+3  1.496
  //工作电极的偏置电压第一个DAC,电压控制信号
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_DINWE_L8);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CH1_DINWE_H2);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //配置CE[0:7]+[8:9]
  //参比电极的偏置电压由第二个DAC,，电压控制信号
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5C);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x7f);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //控制ADC使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x62);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
//  USART_SpiTransfer(USART0, 0x03);
  USART_SpiTransfer(USART0, 0x02);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //DC CLK  //模拟保持分频&关闭BG
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3C);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x7);   //分频系数
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xBE);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  BMS3_Driver_log_info("CFG_KEEP_ANA FINISH!\r\n");
}

void BMS3_Driver_CFG_IMEAS(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;
  ////////////////////////222222222222使能IMEAS_EN
  //IMEAS_EN
  //address:0x18
  //cmd:0x80
  //data:0x1
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_EN);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////3333333333333
  //INPUT_FORMAT
  //address:0x17
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_INPUT_FORMAT);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;


  ////////////////////////4444444444444  CIC
  // IMEAS_REG_CTRL_0
  //address:0x01
  //cmd:0x80
  //data:0x21
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_CTRL_0);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CIC);     //CIC2048
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////55555555555555 通道加模式配置 0单 1连续   通道5 DDA
  //55555555555555   模式配置 0单 1连续         (0:1)
  //通道1正常DDA采集  通道5关闭DDA开启ELE采集  (4:7)
  //IMEAS_REG_CH
  //address:0x03
  //cmd:0x80
  //data:0x1
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_CH);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CHA_NUM);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////77777777777777
  //SD16RST
  //address:0x05
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x0);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x73);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Check data written:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Feedback resistor:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("DDA gain:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage low 8 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage high 2 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("PCLK&ICLK setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("CIC setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("bg,dac enable status:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3b);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  BMS3_Driver_log_info("CFG IMEAS FINISH!\r\n");
}

//1.5V待机功耗
void BMS3_Driver_CFG_TEST(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;
  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  ////////////////////////配置ICLK PCLK 分频
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x3E);   //分频系数
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  //配置DC 分频
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3C);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x7);   //分频系数
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xBE);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //读寄存器值
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info(":%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3C);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info(":%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;
  BMS3_Driver_log_info("CFG_TEST FINISH!\r\n");
}

void BMS3_Driver_CFG_BURST_ANA(void)
{
  BMS3_SPI_CS_PIN_H;

//  //CLK_CTRL_REG
//  //address:0x3A
//  //cmd:0x80
//  //data:0x1
//  //16   1E    26   2E   36    3E
//  //32K  16K   8k   4K   2K    1K
//  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
//  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CLK);
  USART_SpiTransfer(USART0, 0xb);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x03);                  //50使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  USART_SpiTransfer(USART0, 0x00);                  //51默认为0
  USART_SpiTransfer(USART0, 0x00);                  //52默认为0
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);       //53配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);     //54配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  USART_SpiTransfer(USART0, 0x00);                  //55默认为0
  USART_SpiTransfer(USART0, 0x00);                  //56默认为0
  USART_SpiTransfer(USART0, 0x1);                   //57参比电极以及辅助电极使能
  USART_SpiTransfer(USART0, 0x1);                   //58工作电极的偏置电压由第一个DAC生成使能
  USART_SpiTransfer(USART0, CH1_DINWE_L8);          //59设置偏置电压[0:7]
  USART_SpiTransfer(USART0, CH1_DINWE_H2);          //5A设置偏置电压[8:9]
  USART_SpiTransfer(USART0, 0x1);                   //5B参比电极的偏置电压由第二个DAC生成使能[0:7]
  USART_SpiTransfer(USART0, 0x7f);                  //5C配置CE[8:9]
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //控制ADC使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x62);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xBE);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  BMS3_Driver_log_info("CFG_BURST_ANA FINISH!\r\n");



}

void BMS3_Driver_CFG_BURST_IMEAS(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CIC);         //1CIC2048
  USART_SpiTransfer(USART0, 0x00);        //2默认0
  USART_SpiTransfer(USART0, CHA_NUM);     //3通道加模式配置
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////
  //SD16RST
  //address:0x05
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x0);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(17);
  USART_SpiTransfer(USART0, 0x17);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x1);         //17INPUT_FORMAT
  USART_SpiTransfer(USART0, 0x1);        //18使能IMEAS_EN
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;


  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  #if  0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x73);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Check data written:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("feekback register:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("DDA gain:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage low 8 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage high 2 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("PCLK&ICLK setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("CIC setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("bg,dac enable status:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3b);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  BMS3_Driver_log_info("CFG BURST IMEAS FINISH!\r\n");
  #else
  #endif
}

void BMS3_Driver_CFG_BURST_ALL(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;

  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  //16   1E    26   2E   36    3E
  //32K  16K   8k   4K   2K    1K
  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CLK);
  USART_SpiTransfer(USART0, 0xb);
  USART_SpiTransfer(USART0, 0x7);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x03);                  //50使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  USART_SpiTransfer(USART0, 0x00);                  //51默认为0
  USART_SpiTransfer(USART0, 0x00);                  //52默认为0
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);       //53配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);     //54配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  USART_SpiTransfer(USART0, 0x00);                  //55默认为0
  USART_SpiTransfer(USART0, 0x00);                  //56默认为0
  USART_SpiTransfer(USART0, 0x1);                   //57参比电极以及辅助电极使能
  USART_SpiTransfer(USART0, 0x1);                   //58工作电极的偏置电压由第一个DAC生成使能
  USART_SpiTransfer(USART0, CH1_DINWE_L8);          //59设置偏置电压[0:7]
  USART_SpiTransfer(USART0, CH1_DINWE_H2);          //5A设置偏置电压[8:9]
  USART_SpiTransfer(USART0, 0x1);                   //5B参比电极的偏置电压由第二个DAC生成使能[0:7]
  USART_SpiTransfer(USART0, 0xd3);                  //5C配置CE[0:7]
  USART_SpiTransfer(USART0, 0x00);                  //5C配置CE[8:9]
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //ELE_BUF使能+模式    we1 f工作电极1之电压   we1out d 工作电极1之运放输出电压     通道:101：缓冲器（ELE_BUFFER_OUT）之输出
  sl_udelay_wait(10000);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x61);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, ELE_BUF);    //61   f
  USART_SpiTransfer(USART0, 0x1);     //  62 ADC使能
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CLK|0x80);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  // BMS3_Driver_log_info("CFG_BURST_ANA FINISH!\r\n");

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CIC);         //1CIC2048
  USART_SpiTransfer(USART0, 0x00);        //2默认0
  USART_SpiTransfer(USART0, CHA_NUM);     //3通道加模式配置
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////
  //SD16RST
  //address:0x05
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x0);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(17);
  USART_SpiTransfer(USART0, 0x17);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x1);         //17INPUT_FORMAT
  USART_SpiTransfer(USART0, 0x1);        //18使能IMEAS_EN
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;
  // BMS3_Driver_log_info("CFG BURST IMEAS FINISH!\r\n");
  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  #if  1
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x73);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Check data written:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("feekback register:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("DDA gain:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage low 8 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage high 2 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("PCLK&ICLK setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("CIC setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("bg,dac enable status:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3b);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

//  sl_udelay_wait(5);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x3F);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x3F:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;

//  sl_udelay_wait(5);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x40);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x40:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(5);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x41);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x41:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(5);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x42);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x42:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(5);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x45);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x45:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
  #else
  #endif
  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
}

void BMS3_Driver_CFG_BURST_KEEP_ANA(void)
{
  BMS3_SPI_CS_PIN_H;
  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  //16   1E    26   2E   36    3E
  //32K  16K   8k   4K   2K    1K
  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CLK);   //3A
  USART_SpiTransfer(USART0, 0xb);   //3B
  USART_SpiTransfer(USART0, 0x7);   //3C DC CLK分频系数
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x03);                  //50使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  USART_SpiTransfer(USART0, 0x00);                  //51默认为0
  USART_SpiTransfer(USART0, 0x00);                  //52默认为0
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);       //53配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);     //54配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  USART_SpiTransfer(USART0, 0x00);                  //55默认为0
  USART_SpiTransfer(USART0, 0x00);                  //56默认为0
  USART_SpiTransfer(USART0, 0x1);                   //57参比电极以及辅助电极使能
  USART_SpiTransfer(USART0, 0x1);                   //58工作电极的偏置电压由第一个DAC生成使能
  USART_SpiTransfer(USART0, CH1_DINWE_L8);          //59设置偏置电压[0:7]
  USART_SpiTransfer(USART0, CH1_DINWE_H2);          //5A设置偏置电压[8:9]
  USART_SpiTransfer(USART0, 0x1);                   //5B参比电极的偏置电压由第二个DAC生成使能[0:7]
  USART_SpiTransfer(USART0, 0x7f);                  //5C配置CE[8:9]
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //控制ADC使能
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x62);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x1);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0xBE);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  BMS3_Driver_log_info("CFG_BURST_ANA FINISH!\r\n");

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CIC);         //1CIC2048
  USART_SpiTransfer(USART0, 0x00);        //2默认0
  USART_SpiTransfer(USART0, CHA_NUM);     //3通道加模式配置
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////
  //SD16RST
  //address:0x05
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x0);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(5);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(17);
  USART_SpiTransfer(USART0, 0x17);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x1);         //17INPUT_FORMAT
  USART_SpiTransfer(USART0, 0x1);        //18使能IMEAS_EN
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;
  BMS3_Driver_log_info("CFG BURST IMEAS FINISH!\r\n");

}

void BMS3_Driver_CFG_BURST_ALL_SECOND(void)
{
  int buf1 = 0;
  BMS3_SPI_CS_PIN_H;
  //CLK_CTRL_REG
  //address:0x3A
  //cmd:0x80
  //data:0x1
  //16   1E    26   2E   36    3E
  //32K  16K   8k   4K   2K    1K
  ////////////////////////配置ICLK[3:5] PCLK[1:2] 分频 FCLK使能[0]
  //0x3B写B & 0x3A[0]置0
  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CLK);    //3a  CLK
  USART_SpiTransfer(USART0, 0xb);    //3b
  USART_SpiTransfer(USART0, 0x7);    //3c  DCDC
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x03);                  //50使能BG,DAC[0:1]   //模拟保持状态关闭BG，DCDC分频
  USART_SpiTransfer(USART0, 0x00);                  //51默认为0
  USART_SpiTransfer(USART0, 0x00);                  //52默认为0
  USART_SpiTransfer(USART0, CH1_WE1_RFB_SEL);       //53配置反馈电阻[2:5]  使能位WE1&DDA  [0:1]
  USART_SpiTransfer(USART0, CH1_WE1_VGAIN_SEL);     //54配置DDA增益倍数  0 8 10 18 20 28 30 38    1 2 3 4 8 15 22 29
  USART_SpiTransfer(USART0, 0x00);                  //55默认为0
  USART_SpiTransfer(USART0, 0x00);                  //56默认为0
  USART_SpiTransfer(USART0, 0x1);                   //57参比电极以及辅助电极使能
  USART_SpiTransfer(USART0, 0x1);                   //58工作电极的偏置电压由第一个DAC生成使能
  USART_SpiTransfer(USART0, CH1_DINWE_L8);          //59设置偏置电压[0:7]
  USART_SpiTransfer(USART0, CH1_DINWE_H2);          //5A设置偏置电压[8:9]
  USART_SpiTransfer(USART0, 0x1);                   //5B参比电极的偏置电压由第二个DAC生成使能[0:7]
  USART_SpiTransfer(USART0, 0x7f);                  //5C配置CE[0:7]
  USART_SpiTransfer(USART0, 0x00);                  //5C配置CE[8:9]
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //ELE_BUF使能+模式    we1 f工作电极1之电压   we1out d 工作电极1之运放输出电压     通道:101：缓冲器（ELE_BUFFER_OUT）之输出
  sl_udelay_wait(10000);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x61);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0xd);    //61   f
  USART_SpiTransfer(USART0, 0x1);     //  62 ADC使能
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  //第7位置1  3E|80
  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, CLK|0x80);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

//  BMS3_Driver_log_info("CFG_BURST_ANA FINISH!\r\n");

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(15);
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, CIC);          //1CIC2048
  USART_SpiTransfer(USART0, 0x00);        //2默认0
  USART_SpiTransfer(USART0, CHA_NUM);     //3通道加模式配置
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;

  ////////////////////////
  //SD16RST
  //address:0x05
  //cmd:0x80
  //data:0x0
  ////////////////////////
  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, IMEAS_REG_SEQ);
  USART_SpiTransfer(USART0, WR_SINGLE_CMD);
  USART_SpiTransfer(USART0, 0x0);
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  sl_udelay_wait(17);
  USART_SpiTransfer(USART0, 0x17);
  USART_SpiTransfer(USART0, WR_BURST_CMD);
  USART_SpiTransfer(USART0, 0x1);          //17INPUT_FORMAT
  USART_SpiTransfer(USART0, 0x1);        //18使能IMEAS_EN
  USART_SpiTransfer(USART0, PAD);
  sl_udelay_wait(30);
  BMS3_SPI_CS_PIN_H;
  sl_udelay_wait(1);
//  BMS3_Driver_log_info("CFG BURST IMEAS FINISH!\r\n");
  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
  #if  0
  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x73);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Check data written:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x53);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Feedback resistor:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x54);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("DDA gain:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x59);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage low 8 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x5A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("Bias voltage high 2 bits:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3A);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("PCLK&ICLK setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x01);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("CIC setting:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x50);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("bg,dac enable status:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x3b);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("3B:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

  sl_udelay_wait(3);
  BMS3_SPI_CS_PIN_L;
  USART_SpiTransfer(USART0, 0x61);
  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("addr:%x\n",buf1);
  buf1 = USART_SpiTransfer(USART0, PAD);
  BMS3_Driver_log_info("0x61:%x\n",buf1);
  sl_udelay_wait(1);
  BMS3_SPI_CS_PIN_H;

//  sl_udelay_wait(3);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x40);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x40:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(3);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x41);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x41:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(3);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x42);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x42:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
//
//  sl_udelay_wait(3);
//  BMS3_SPI_CS_PIN_L;
//  USART_SpiTransfer(USART0, 0x45);
//  USART_SpiTransfer(USART0, RD_SINGLE_CMD);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("addr:%x\n",buf1);
//  buf1 = USART_SpiTransfer(USART0, PAD);
//  BMS3_Driver_log_info("0x45:%x\n",buf1);
//  sl_udelay_wait(1);
//  BMS3_SPI_CS_PIN_H;
  #else
  #endif
  ////////////////////////////////////读寄存器值//////////////////////////////////////////////////////////////////
}
