/*
 * AFE.h
 *
 *  Created on: 2024年5月13日
 *      Author: SZZ0030
 */

#ifndef INCLUDE_AFE_H_
#define INCLUDE_AFE_H_


#include "em_common.h"

#define BMS3_DRIVER_INT_EVT 0x1

#ifdef old_version
#define  IMEAS_REG_CTRL_0           0x01
#define  IMEAS_REG_CTRL_1           0x02
#define  IMEAS_REG_CH               0x03
#define  IMEAS_INT                  0x04
#define  IMEAS_REG_SEQ              0x05
#define  IMEAS_REG_RSTVAL           0x06
#define  IMEAS_CH0DATA_0            0x07
#define  IMEAS_CH0DATA_1            0x08
#define  IMEAS_CH1DATA_0            0x09
#define  IMEAS_CH1DATA_1            0x0a
#define  IMEAS_CH2DATA_0            0x0b
#define  IMEAS_CH2DATA_1            0x0c
#define  IMEAS_GRP_CTL              0x0d
#define  IMEAS_CHA_NUM_LO           0x0e
#define  IMEAS_CHA_NUM_HI           0x0f
#define  IMEAS_EN                   0x11

#define  ZMEAS_REG_CTRL_0           0x12
#define  ZMEAS_REG_CTRL_1           0x13
#define  ZMEAS_REG_CTRL_2           0x14
#define  ZMEAS_REG_CTRL_3           0x15
#define  ZMEAS_REG_STATUS_0         0x16
#define  ZMEAS_REG_STATUS_1         0x17
#define  ZMEAS_REG_DATAOUT_0        0x18
#define  ZMEAS_REG_DATAOUT_1        0x19
#define  ZMEAS_REG_DATAOUT_2        0x1A
#define  ZMEAS_REG_DATAOUT_3        0x1B
#define  ZMEAS_ADC_ROM_REG_0        0x1C
#define  ZMEAS_ADC_ROM_REG_1        0x1D
#define  ZMEAS_ADC_ROM_REG_2        0x1E
#define  ZMEAS_ADC_ROM_REG_3        0x1F
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_0 0x20
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_1 0x21
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_2 0x22
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_3 0x23
#define  ZMEAS_SUMMATION_REAL_0     0x24
#define  ZMEAS_SUMMATION_REAL_1     0x25
#define  ZMEAS_SUMMATION_REAL_2     0x26
#define  ZMEAS_SUMMATION_REAL_3     0x27
#define  ZMEAS_SUMMATION_IMAG_0     0x28
#define  ZMEAS_SUMMATION_IMAG_1     0x29
#define  ZMEAS_SUMMATION_IMAG_2     0x2A
#define  ZMEAS_SUMMATION_IMAG_3     0x2B
#define  ZMEAS_SUMMATION_SHIFT_0    0x2C
#define  ZMEAS_SUMMATION_SHIFT_1    0x2D
#define  ZMEAS_SUMMATION_SHIFT_2    0x2E
#define  ZMEAS_SUMMATION_SHIFT_3    0x2F
#define  ZMEAS_INT                  0x30
#define  ZMEAS_ADC_INT              0x31

#define  MEAS_SYNC_EN               0x32

#define  IMEAS_ALARM_INT            0x33
#define  IMEAS_ALARM_INT_EN         0x34
#define  IMEAS_THRESHOLD_HI_0       0x35
#define  IMEAS_THRESHOLD_HI_1       0x36
#define  IMEAS_THRESHOLD_LO_0       0x37
#define  IMEAS_THRESHOLD_LO_1       0x38
#define  IMEAS_INPUT_FORMAT         0x39
#define  ZMEAS_EN                   0x3A

#define  CLK_CTRL_REG               0x3B
#define  PMU_REG0                   0x3C

#define  ALWAYS_ON_CLK_CTRL         0x3D

//#define  FLASH_CONFIG       0x3e
#define  FLASH_DEBUG1               0x3f
#define  FLASH_DEBUG2               0x40

#define  FLASH_TRIMDATA0            0x41
#define  FLASH_TRIMDATA1            0x42
#define  FLASH_TRIMDATA2            0x43
#define  FLASH_TRIMDATA3            0x44
#define  FLASH_TRIMDATA4            0x45
#define  FLASH_TRIMDATA5            0x46
#define  FLASH_TRIMDATA6            0x47
//#define  FLASH_TRIMDATA7    0x48

#define  FIFO_WR_PTR_REG    0x49
#define  FIFO_RD_PTR_REG    0x4a
#define  FIFO_COUNTER_1_REG 0x4b
#define  FIFO_COUNTER_2_REG 0x4c
#define  FIFO_CONFIG_1_REG  0x4d
#define  FIFO_CONFIG_2_REG  0x4e
#define  FIFO_CONFIG_3_REG  0x4f
#define  FIFO_STATUS_REG    0x50

#define  FIFO_DATA_REG1     0x51
#define  FIFO_DATA_REG2     0x52



#else


#define  IMEAS_REG_CTRL_0   0x01
#define  IMEAS_REG_CTRL_1   0x02
#define  IMEAS_REG_CH       0x03
#define  IMEAS_INT          0x04
#define  IMEAS_REG_SEQ      0x05
#define  IMEAS_REG_RSTVAL   0x06
#define  IMEAS_CH0DATA_0    0x07
#define  IMEAS_CH0DATA_1    0x08
#define  IMEAS_CH1DATA_0    0x09
#define  IMEAS_CH1DATA_1    0x0A
#define  IMEAS_CH2DATA_0    0x0B
#define  IMEAS_CH2DATA_1    0x0C
#define  IMEAS_GRP_CTL          0x0D
#define  IMEAS_CHA_NUM_LO       0x0E
#define  IMEAS_CHA_NUM_HI       0x0F
#define  MEAS_SYNC_EN           0x10

#define  IMEAS_ALARM_INT    0x11
#define  IMEAS_ALARM_INT_EN     0x12
#define  IMEAS_THRESHOLD_HI_0     0x13
#define  IMEAS_THRESHOLD_HI_1     0x14
#define  IMEAS_THRESHOLD_LO_0     0x15
#define  IMEAS_THRESHOLD_LO_1     0x16
#define  IMEAS_INPUT_FORMAT     0x17
#define  IMEAS_EN                   0x18



#define  ZMEAS_REG_CTRL_0   0x19
#define  ZMEAS_REG_CTRL_1   0x1A
#define  ZMEAS_REG_CTRL_2   0x1B
#define  ZMEAS_REG_CTRL_3   0x1C
#define  ZMEAS_REG_STATUS_0   0x1D
#define  ZMEAS_REG_STATUS_1   0x1E
#define  ZMEAS_REG_DATAOUT_0  0x1F
#define  ZMEAS_REG_DATAOUT_1  0x20
#define  ZMEAS_REG_DATAOUT_2  0x21
#define  ZMEAS_REG_DATAOUT_3  0x22

#define  ZMEAS_ADC_ROM_REG_0  0x23 //24
#define  ZMEAS_ADC_ROM_REG_1  0x24
#define  ZMEAS_ADC_ROM_REG_2  0x25
#define  ZMEAS_ADC_ROM_REG_3  0x26
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_0   0x27
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_1   0x28
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_2   0x29
#define  ZMEAS_SUMMATION_OFFSET_FORREAL_3   0x2A
#define  ZMEAS_SUMMATION_REAL_0         0x2B
#define  ZMEAS_SUMMATION_REAL_1         0x2C
#define  ZMEAS_SUMMATION_REAL_2         0x2D
#define  ZMEAS_SUMMATION_REAL_3         0x2E
#define  ZMEAS_SUMMATION_IMAG_0         0x2F
#define  ZMEAS_SUMMATION_IMAG_1         0x30
#define  ZMEAS_SUMMATION_IMAG_2         0x31
#define  ZMEAS_SUMMATION_IMAG_3         0x32
#define  ZMEAS_SUMMATION_SHIFT_0        0x33
#define  ZMEAS_SUMMATION_SHIFT_1        0x34
#define  ZMEAS_SUMMATION_SHIFT_2        0x35
#define  ZMEAS_SUMMATION_SHIFT_3        0x36
#define  ZMEAS_INT                      0x37
#define  ZMEAS_ADC_INT                  0x38
#define  ZMEAS_EN                       0x39


#define  CLK_CTRL_REG                   0x3A
#define  PMU_REG0                       0x3B

#define  ALWAYS_ON_CLK_CTRL             0x3C

//`define  FLASH_CONFIG                   0x3E
#define  FLASH_DEBUG1                   0x3D
#define  FLASH_DEBUG2                   0x3E

#define  FLASH_TRIMDATA0                 0x3F
#define  FLASH_TRIMDATA1                 0x40
#define  FLASH_TRIMDATA2                 0x41
#define  FLASH_TRIMDATA3                 0x42
#define  FLASH_TRIMDATA4                 0x43
#define  FLASH_TRIMDATA5                 0x44
#define  FLASH_TRIMDATA6                 0x45


//FIFO Registers
#define  FIFO_WR_PTR_REG                0x46
#define  FIFO_RD_PTR_REG                0x47
#define  FIFO_COUNTER_1_REG             0x48
#define  FIFO_COUNTER_2_REG             0x49
#define  FIFO_CONFIG_1_REG              0x4A
#define  FIFO_CONFIG_2_REG              0x4B
#define  FIFO_CONFIG_3_REG              0x4C
#define  FIFO_STATUS_REG                0x4D
#define  FIFO_DATA_REG1                 0x4E
#define  FIFO_DATA_REG2                 0x4F




//analog register define
#define  ANA_PMU                        0x50
#define  ANA_TSC_0                      0x51
#define  ANA_TSC_1                      0x52


#define ANA_IMEAS_CH1_WE1_0             0x53
#define ANA_IMEAS_CH1_WE1_1             0x54
#define ANA_IMEAS_CH1_WE2_0             0x55
#define ANA_IMEAS_CH1_WE2_1             0x56
#define ANA_IMEAS_CH1_RCE_ROUTSEL       0x57
#define ANA_IMEAS_CH1_WE_DAC_EN         0x58
#define ANA_IMEAS_CH1_DINWE_0           0x59
#define ANA_IMEAS_CH1_DINWE_1           0x5A
#define ANA_IMEAS_CH1_RCE_DAC_EN        0x5B
#define ANA_IMEAS_CH1_DINRCE_0          0x5C
#define ANA_IMEAS_CH1_DINRCE_1          0x5D


#define ANA_BIST                       0x5E
#define ANA_DDA                        0x5F
#define ANA_PGA                        0x60
#define ANA_ELE                        0x61

#define ANA_SDM                        0x62


//xin add 2/Oct/2022
#define ANA_Z_ADC_DAC_EN               0x63
#define ANA_Z_ADC_DAC_EN_SEL           0x64

//device status
#define DEVICE_INT_STATUS_0           0x65
#define DEVICE_INT_STATUS_1           0x66

//Analog debug regsiters
#define  ALWAYS_ON_CLK_CTRL_DEBUG                 0x67

#define ALWAYS_ON_ANA_IMEAS_CH1_WE1_0_DEBUG       0x68
#define ALWAYS_ON_ANA_IMEAS_CH1_WE1_1_DEBUG       0x69
#define ALWAYS_ON_ANA_IMEAS_CH1_WE2_0_DEBUG       0x6A
#define ALWAYS_ON_ANA_IMEAS_CH1_WE2_1_DEBUG       0x6B
#define ALWAYS_ON_ANA_IMEAS_CH1_RCE_ROUTSEL_DEBUG 0x6C
#define ALWAYS_ON_ANA_IMEAS_CH1_WE_DAC_EN_DEBUG   0x6D
#define ALWAYS_ON_ANA_IMEAS_CH1_DINWE_0_DEBUG     0x6E
#define ALWAYS_ON_ANA_IMEAS_CH1_DINWE_1_DEBUG     0x6F
#define ALWAYS_ON_ANA_IMEAS_CH1_RCE_DAC_EN_DEBUG  0x70
#define ALWAYS_ON_ANA_IMEAS_CH1_DINRCE_0_DEBUG    0x71
#define ALWAYS_ON_ANA_IMEAS_CH1_DINRCE_1_DEBUG    0x72


#define ALWAYS_ON_ANA_PMU_DEBUG                   0x73


#define ALWAYS_ON_ANA_BIST_DEBUG                 0x74
#define ALWAYS_ON_ANA_DDA_DEBUG                  0x75
#define ALWAYS_ON_ANA_ELE_DEBUG                  0x76


#define ALWAYS_ON_ANA_BGH_VTRIM_DEBUG           0x77
#define ALWAYS_ON_ANA_BGH_CTRIM_DEBUG           0x78
#define ALWAYS_ON_ANA_BGL_VTRIM_DEBUG           0x79
#define ALWAYS_ON_ANA_BGL_CTRIM_DEBUG           0x7A
#define ALWAYS_ON_ANA_LDOL1V5_TRIM_DEBUG        0x7B
#define ALWAYS_ON_ANA_DAC_BUF_TRIM_DEBUG        0x7C
#define ALWAYS_ON_ANA_OSC_TRIM_DEBUG            0x7D

#endif

#define  WR_BURST_CMD       0xA0
#define  WR_SINGLE_CMD      0x80
#define  RD_SINGLE_CMD      0x0
#define  RD_BURST_FIFO_CMD  0x60
#define  RD_BURST_REG_CMD   0x20

#define  PAD                0x0

////ANA参数
////配置反馈电阻   (2:5) 3     13    1B   23    27    2B   2f   33   37   3B   3f       39
////(0:1)WE1&DDA_EN     50K   500K  1M   3M    4M    5M   6M   7M   8M   10M  无穷大   10M关闭DDA(第1位使能DDA)
//#define CH1_WE1_RFB_SEL  0x1B
extern uint8_t CH1_WE1_RFB_SEL;
////配置DDA增益倍数  0 8 10 18 20 28 30 38
//// (3:5)          1 2  3  4  8 15 22 29
//#define CH1_WE1_VGAIN_SEL  0x0
extern uint8_t CH1_WE1_VGAIN_SEL;
////设置偏置电压 (0:7) (8:11)
////0  0.3  3f 0.4  7f 0.511  ff 0.724
////1+3f  0.83V  1+8f 0.963  1+df 1.096  1+ff  1.149   2+f 1.176   2+3f 1.256
////2+7f 1.363   2+ff  1.489   3+ff  1.496

//#define CH1_DINWE_L8  0xff
//#define CH1_DINWE_H2  0x3
extern uint8_t CH1_DINWE_L8;
extern uint8_t CH1_DINWE_H2;
////ELE_BUF使能+模式    we1 f工作电极1之电压   we1out d 工作电极1之运放输出电压    通道:101(5)：缓冲器（ELE_BUFFER_OUT）之输出   (1:3)
#define ELE_BUF  0xf

////IMEAS参数
////55555555555555   模式配置 0单 1连续         (0:1)
////通道1正常DDA采集  通道5关闭DDA开启ELE采集  (4:7)
#define CHA_NUM  0x54

extern uint8_t CLK;
extern uint8_t CIC;

void BMS3_Driver_Init(void);
float BMS3_Driver_EXTI_IMEAS_INT_CFG(void);
void BMS3_Driver_CFG_BURST_KEEP_ANA(void);
void BMS3_Driver_CFG_BURST_ALL(void);
void BMS3_Driver_CFG_BURST_IMEAS(void);
void BMS3_Driver_CFG_BURST_ANA(void);
void BMS3_Driver_CFG_IMEAS(void);
void BMS3_Driver_CFG_ANA(void);
void BMS3_Driver_CFG_TEST(void);
void BMS3_Driver_CFG_KEEP_ANA(void);
void BMS3_Driver_CFG_BURST_ALL_SECOND(void);



#define BSP_GPIO_CHIP_WAKEUP_ENA_PORT  gpioPortB
#define BSP_GPIO_CHIP_WAKEUP_ENA_PIN    0


#define BSP_GPIO_CHIP_ENA_PORT    gpioPortB
#define BSP_GPIO_CHIP_ENA_PIN     1




#define Afe_Chip_Enable()          GPIO_PinOutSet(BSP_GPIO_CHIP_ENA_PORT,BSP_GPIO_CHIP_ENA_PIN)
#define Afe_Chip_Disable()         GPIO_PinOutClear(BSP_GPIO_CHIP_ENA_PORT,BSP_GPIO_CHIP_ENA_PIN)

#define Afe_Chip_Wakeup_Enable()    GPIO_PinOutSet(BSP_GPIO_CHIP_ENA_PORT,BSP_GPIO_CHIP_ENA_PIN)
#define Afe_Chip_Wakeup_Disable()   GPIO_PinOutClear(BSP_GPIO_CHIP_WAKEUP_ENA_PORT,BSP_GPIO_CHIP_WAKEUP_ENA_PIN)





void AFE_Init_Gpio();

#endif /* INCLUDE_AFE_H_ */
