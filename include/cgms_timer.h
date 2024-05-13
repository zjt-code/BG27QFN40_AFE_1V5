/*
 * cgms_timer.h
 *
 *  Created on: 2024年5月11日
 *      Author: SZZ0030
 */

#ifndef   INCLUDE_CGMS_TIMER_H_
#define   INCLUDE_CGMS_TIMER_H_


#define   SENSOR_SAMPLE_INTERVAL  32768

#define   SLEEP_TIMER_WAKEUP_EVT 0x02


#define   LE_MONITOR_SIGNAL      0x01



#define   BSP_GPIO_LED1_PORT gpioPortB
#define   BSP_GPIO_LED1_PIN  0




int cgms_timer_start(void);



#endif /* INCLUDE_CGMS_TIMER_H_ */
