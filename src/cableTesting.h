/** 
  ******************************************************************************
  * @file    waCamEmu.h
  * @author  Gremsy Team
  * @version v1.0.0
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2011 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __WACAMEMU_H
#define __WACAMEMU_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include "stdbool.h"
/* Exported define ------------------------------------------------------------*/

#define MAX_READ_DATA_SIZE 1000
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    CABLE_TESTING_STATE_IDLE = 0x00,
    CABLE_TESTING_STATE_RUNNING,
    CABLE_TESTING_STATE_CHECK_RESULT,
    CABLE_TESTING_STATE_DONE,
    CABLE_TESTING_STATE_ERROR,
    CABLE_TESTING_TOTAL_STATE

}cableTesting_state_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void cableTesting_init(void);
void cableTesting_SendDataProcess(void);
void cableTesting_UsbReadProcess(void);
void cableTesting_UartReadProcess(void);

void cableTesting_manager(void);
void cableTesting_ledStatus(void);
cableTesting_state_t cableStatus();

#endif /*  */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

