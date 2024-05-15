/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include <afe.h>
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app.h"
#include "cgms_timer.h"
#include "em_cmu.h"
#include "cgms_timer.h"
#include "afe.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
  ///
    CMU_ClockEnable(cmuClock_GPIO, true);
  //start up cgms timer

    AFE_Init_Gpio();
    BMS3_Driver_Init();
    cgms_timer_start();

}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Create an advertising set.

      int16_t power=0;
      int16_t set_power=0;


      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      sl_bt_advertiser_set_tx_power(advertising_set_handle, power, &set_power);

      // Set advertising interval to 1000ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        1600, // min. adv. interval (milliseconds * 1.6)
        1600, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_legacy_advertiser_connectable);
      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////
    case sl_bt_evt_system_external_signal_id:
        if((evt->data.evt_system_external_signal.extsignals == SLEEP_TIMER_WAKEUP_EVT))
          {
            app_log_info("sleep timer wakeup evt\r\n");

             BMS3_Driver_Init();
//
//            float AFE_value = BMS3_Driver_EXTI_IMEAS_INT_CFG();
//                      app_log_info("AFE_VALUE = %d\r\n",(int32_t)(AFE_value*100));
          }

        if((evt->data.evt_system_external_signal.extsignals == BMS3_DRIVER_INT_EVT))
          {

          //  BMS3_Driver_Init();

           float AFE_value = BMS3_Driver_EXTI_IMEAS_INT_CFG();
           app_log_info("AFE_VALUE = %d\r\n",(int32_t)(AFE_value*100));
          }


      break;

    case sl_bt_evt_gatt_server_attribute_value_id:


         app_log_info("char handle=%d,data_len=%d\r\n",evt->data.evt_gatt_server_attribute_value.attribute,\
                      evt->data.evt_gatt_server_attribute_value.offset);
         // The value of the gattdb_led_control characteristic was changed.
         if (gattdb_led_control == evt->data.evt_gatt_server_attribute_value.attribute) {
           uint8_t data_recv;
           size_t data_recv_len;
           // Read characteristic value.
           sc = sl_bt_gatt_server_read_attribute_value(gattdb_led_control,
                                                       0,
                                                       sizeof(data_recv),
                                                       &data_recv_len,
                                                       &data_recv);
           app_log_info("len =%d\r\n",data_recv_len);
           (void)data_recv_len;
           app_log_status_error(sc);

           if (sc != SL_STATUS_OK) {
             break;
           }

           // Toggle LED.
           if (data_recv == 0x00) {
           // sl_led_turn_off(SL_SIMPLE_LED_INSTANCE(0));
//               GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
               GPIO_PinOutClear(BSP_GPIO_HALL_LOCK_PORT,BSP_GPIO_HALL_LOCK_PIN);
               Afe_Chip_Disable();

             app_log_info("HALL_Lock off.\n");
           } else if (data_recv == 0x01) {
           //  sl_led_turn_on(SL_SIMPLE_LED_INSTANCE(0));
//               GPIO_PinOutGet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
               GPIO_PinOutSet(BSP_GPIO_HALL_LOCK_PORT,BSP_GPIO_HALL_LOCK_PIN);
               Afe_Chip_Enable();
             app_log_info("HALL on.\n");
           } else {
             app_log_error("Invalid attribute value: 0x%02x\n", (int)data_recv);
           }
         }
         break;
    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
