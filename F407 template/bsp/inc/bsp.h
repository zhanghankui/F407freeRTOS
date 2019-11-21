/*
*********************************************************************************************************
* @file    bsp.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    24-May-2018 22:09
* @brief   这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*          不需要#include 每个模块的 h 文件
* Update record
* version   date           author          description
* V1.00     24-May-2018    smilingfrog     first release
*********************************************************************************************************
*/


#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V5
//#define STM32_X3

/* 检查是否定义了开发板型号 */
#if !defined (STM32_V5) && !defined (STM32_X3)
	#error "Please define the board model : STM32_X3 or STM32_V5"
#endif

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
	#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* 开关全局中断的宏 */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#endif

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/*
	EXTI9_5_IRQHandler 的中断服务程序分散在几个独立的 bsp文件中。
	需要整合到 stm32f4xx_it.c 中。

	定义下面行表示EXTI9_5_IRQHandler入口函数集中放到 stm32f4xx_it.c。
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1	/* 打印GPS数据到串口1 */

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */

#include "led.h"
#include "usartdma.h"
//#include "bsp_timer.h"
#include "bsp_key.h"
#include "sdio_sdcard.h"
#include "w25qxx.h"
//#include "bsp_can.h"
//#include "bsp_msg.h"

#include "bsp_tim_pwm.h"

//#include "bsp_cpu_flash.h"
//#include "bsp_sdio_sd.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_eeprom_24xx.h"
//#include "bsp_si4730.h"
//#include "bsp_hmc5883l.h"
//#include "bsp_mpu6050.h"
//#include "bsp_bh1750.h"
//#include "bsp_bmp085.h"
//#include "bsp_wm8978.h"
//#include "bsp_gt811.h"

//#include "bsp_fsmc_sram.h"
//#include "bsp_nand_flash.h"
//#include "bsp_nor_flash.h"

//#include "LCD_RA8875.h"
//#include "LCD_SPFD5420.h"
//#include "LCD_ILI9488.h"
//#include "bsp_ra8875_port.h"
//#include "bsp_tft_lcd.h"

//#include "bsp_touch.h"
//#include "bsp_camera.h"

//#include "bsp_ad7606.h"

//#include "bsp_gps.h"
//#include "bsp_oled.h"
////#include "bsp_mg323.h"
//#include "bsp_sim800.h"

//#include "bsp_ra8875_flash.h"

//#include "bsp_spi_bus.h"
//#include "bsp_spi_flash.h"
//#include "bsp_tm7705.h"
//#include "bsp_ads1256.h"
//#include "bsp_vs1053b.h"
//#include "bsp_tsc2046.h"
//#include "bsp_ts_ft5x06.h"

//#include "bsp_ds18b20.h"
//#include "bsp_dac8501.h"
//#include "bsp_dht11.h"

//#include "bsp_ir_decode.h"
//#include "bsp_ps2.h"

//#include "bsp_modbus.h"
//#include "bsp_rs485_led.h"
//#include "bsp_user_lib.h"

//#include "bsp_dac8501.h"
//#include "bsp_dac8562.h"
//#include "bsp_beep.h"

//#include "bsp_adc_dso.h"
//#include "bsp_dac_wave.h"

//#include "bsp_esp8266.h"
//#include "bsp_ad9833.h"



//! \brief      Defines the BSP object
//! \details    The BSP object include basic hardware configuration
//!
typedef struct _BSP_Obj_
{
	USARTDMA_FIFO_Handle usartdma_fifohandle;
	USARTDMA_FIFO_Obj    usartdma_fifo;	
}BSP_Obj;

//! \brief Defines the BSP handle
//!
typedef struct _BSP_Obj_ *BSP_Handle;












/* 提供给其他C文件调用的函数 */
void bsp_Init(BSP_Handle handle);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
