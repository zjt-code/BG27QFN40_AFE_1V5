/*
 * cgms_timer.c
 *
 *  Created on: 2024年5月11日
 *      Author: SZZ0030
 */

#include "sl_sleeptimer.h"
#include "app_log.h"
#include "cgms_timer.h"
#include "app_log.h"
#include "em_gpio.h"



static uint32_t times =0;
//static uint32_t b_sample_flag=false;

static void my_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  //Code executed when the timer expire.

//  if(times%2)
//    GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
//  else
//    GPIO_PinOutGet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);

  sl_bt_external_signal(LE_MONITOR_SIGNAL);
  times ++;
  app_log_info("timer out callback times:%lu\r\n", times);
//
}

static sl_sleeptimer_timer_handle_t my_timer;


int cgms_timer_start(void)
{
  sl_status_t status;

  uint32_t timer_timeout = SENSOR_SAMPLE_INTERVAL;


  GPIO_PinModeSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN, gpioModePushPull, 1);

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
