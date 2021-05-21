/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : custom_app.c
 * Description        : Custom Example Application (Server)
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef struct{

	uint16_t TimeStamp;
	uint8_t BPMVal; // Als ik deze nu verander in 2x 8 zou dat werken?
	uint8_t SPO2;
}CUSTOM_VALUES_t;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* BleWrite */
/* USER CODE BEGIN CUSTOM_APP_Context_t */
	CUSTOM_VALUES_t BPMSPO2;
	uint8_t Update_Timer_Id;

/* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HEARTRATE_CHANGE_STEP            10
#define HEARTRATE_CHANGE_PERIOD        (1*1000*1000/CFG_TS_TICK_VAL) /*1000ms*/
#define HEARTRATE_VALUE_MAX_THRESHOLD   350
#define HEARTATE_VALUE_MIN_THRESHOLD   100
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */
uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

uint8_t SecureReadData;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
  /* BleWrite */

/* USER CODE BEGIN PFP */
static void Custom_HeartRateChange_Timer_Callback(void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
/* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
	uint8_t value[4];

	value[0] = (uint8_t) (Custom_App_Context.BPMSPO2.TimeStamp & 0x00FF);
	value[1] = (uint8_t) (Custom_App_Context.BPMSPO2.TimeStamp  >> 8);
	value[2] = (uint8_t) (Custom_App_Context.BPMSPO2.BPMVal);
	value[3] = (uint8_t) (Custom_App_Context.BPMSPO2.SPO2);


	Custom_App_Context.BPMSPO2.BPMVal = 5;
	Custom_App_Context.BPMSPO2.SPO2 = 7;
	Custom_App_Context.BPMSPO2.TimeStamp = HEARTRATE_CHANGE_STEP;


/* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch(pNotification->Custom_Evt_Opcode)
  {
/* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

/* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

  /* BleWrite */
    case CUSTOM_STM_BN_READ_EVT:
/* USER CODE BEGIN CUSTOM_STM_BN_READ_EVT */

    	/* Start timer */
    	HW_TS_Start(Custom_App_Context.Update_Timer_Id,HEARTRATE_CHANGE_PERIOD );

/* USER CODE END CUSTOM_STM_BN_READ_EVT */
      break;

    default:
/* USER CODE BEGIN CUSTOM_STM_App_Notification_default */
    	HW_TS_Stop(Custom_App_Context.Update_Timer_Id);
/* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
/* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

/* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
/* USER CODE BEGIN CUSTOM_APP_Notification_1 */

/* USER CODE END CUSTOM_APP_Notification_1 */

  switch(pNotification->Custom_Evt_Opcode)
  {
/* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

/* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
  case CUSTOM_CONN_HANDLE_EVT :
/* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

/* USER CODE END CUSTOM_CONN_HANDLE_EVT */
    break;

    case CUSTOM_DISCON_HANDLE_EVT :
/* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

/* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
    break;

    default:
/* USER CODE BEGIN CUSTOM_APP_Notification_default */

/* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

/* USER CODE BEGIN CUSTOM_APP_Notification_2 */

/* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
/* USER CODE BEGIN CUSTOM_APP_Init */

	// Hier de 2 parameters initialiseren
	// Timestamp & value

	Custom_App_Context.BPMSPO2.TimeStamp = 0;
	Custom_App_Context.BPMSPO2.BPMVal = 0;
	Custom_App_Context.BPMSPO2.SPO2 = 0;


	UTIL_SEQ_RegTask(CFG_MY_TASK_NOTIFY_HEARTRATE,UTIL_SEQ_RFU,Custom_STM_App_Notification);

	// Timer creeeren voor de update
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,
			&(Custom_App_Context.Update_Timer_Id),
			hw_ts_Repeated,
			Custom_HeartRateChange_Timer_Callback
			);

/* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */


static void Custom_HeartRateChange_Timer_Callback(void)
{
	UTIL_SEQ_SetTask(1<<CFG_MY_TASK_NOTIFY_HEARTRATE, CFG_SCH_PRIO_0);
}


/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

  /* BleWrite */

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
