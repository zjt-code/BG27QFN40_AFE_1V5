/*
 * cgms_timer.c
 *
 *  Created on: 2024年5月11日
 *      Author: SZZ0030
 */

#include "sl_bt_api.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "cgms_timer.h"
#include "app_log.h"
#include "em_gpio.h"



static void GPIO_OutPutTest(GPIO_Port_TypeDef port,unsigned int pin)
{
      static int cnt =0;

      cnt++;
      if(cnt%2)
        {
          GPIO_PinOutClear(port, pin);
          app_log_info("LED OFF\r\n");

        }
      else
        {
          GPIO_PinOutSet(port, pin);
          app_log_info("LED On\r\n");
        }
}


static unsigned int GPIO_InputTest(GPIO_Port_TypeDef port,unsigned int pin)
{

    unsigned int input_value=GPIO_PinInGet(port,pin);

    app_log_info("port[%d],pin[%d] ;inputvalue = %d\r\n",port,pin,input_value);

    return input_value;
}



static uint32_t times =0;
//static uint32_t b_sample_flag=false;

static void my_timer_callback(sl_sleeptimer_timer_handle_t *handle __attribute__((unused)),
                              void *data __attribute__((unused)))
{
  //Code executed when the timer expire.

//  (void *)data;
//  (void *)handle;

  GPIO_OutPutTest(BSP_GPIO_LED1_PORT,BSP_GPIO_LED1_PIN);

  GPIO_InputTest(BSP_GPIO_KEY1_PORT,BSP_GPIO_KEY1_PIN);

  sl_bt_external_signal(SLEEP_TIMER_WAKEUP_EVT);
  times ++;
  app_log_info("timer out callback times:%lu\r\n", times);
//
}

static sl_sleeptimer_timer_handle_t my_timer;


int cgms_timer_start(void)
{
  sl_status_t status;

  uint32_t timer_timeout = SENSOR_SAMPLE_INTERVAL * 20 ;


  GPIO_PinModeSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN, gpioModePushPull, 1);

  GPIO_PinModeSet(BSP_GPIO_KEY1_PORT, BSP_GPIO_KEY1_PIN, gpioModeInputPull, 1);


  GPIO_PinModeSet(BSP_GPIO_HALL_LOCK_PORT,BSP_GPIO_HALL_LOCK_PIN,gpioModePushPull,0);

  GPIO_PinOutSet(BSP_GPIO_HALL_LOCK_PORT,BSP_GPIO_HALL_LOCK_PIN);
//  GPIO_PinOutClear(BSP_GPIO_HALL_LOCK_PORT,BSP_GPIO_HALL_LOCK_PIN);


  // We assume the sleeptimer is initialized properly

  status = sl_sleeptimer_start_periodic_timer(&my_timer,
                                     timer_timeout,
                                     my_timer_callback,
                                     (void *)NULL,
                                     0,
                                     0);
  app_log_info("start_timer_errocode =%ld\r\n",status);

  if(status != SL_STATUS_OK)
  {
    app_log_info("start timer fail\n");
    return -1;
  }
  return 1;
}
