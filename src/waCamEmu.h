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

#define MAX_READ_DATA_SIZE 256
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  CAMERA_LAYOUT_KNOW = 0x00,
  CAMERA_LAYOUT_FULLSCREEN,
  CAMERA_LAYOUT_INSPECTION,
  CAMERA_LAYOUT_PiP,

}waCameraLayoutIndex_t;

typedef enum
{
  MAIN_CAMERA_KNOW = 0x00,
  MAIN_CAMERA_THERMAL,
  MAIN_CAMERA_VISIBLE,

}waCameraMainScreen_t;

typedef enum
{
    CWSI_MODE_UNKNOW = -1,
    CWSI_MODE_EMPIRICAL = 0x00,
    CWSI_MODE_THEORETIC,
    CWSI_MODE_DIFFERENTIAL,
    CWSI_MODE_TOTAL,

}waCameraCwsiMode_t;

typedef enum
{
  CAMERA_WIDGET_TYPE_NONE = 0x00,
  CAMERA_WIDGET_TYPE_01,
  CAMERA_WIDGET_TYPE_02,

}waCameraWidgetType_t;

typedef struct 
{
  /* data */
  float D1;
  float D2;
}waCamEmuConstants_t;

typedef struct
{
  int index;
  int (*Function_int)(char *array, int index);
}waCamEmuResponseCommandHandleList_t;


typedef struct
{
  char readBuffer[MAX_READ_DATA_SIZE];

  int (*sendData)(char *data, int len);
  int (*readData)(char *data, int len);

  int totalResponseCommandNumber;

  waCameraLayoutIndex_t layout;
  waCameraMainScreen_t  mainScreen;
  waCameraCwsiMode_t cwsiMode;

  waCameraWidgetType_t widgetType;

  float thermalZoomValue;
  int visibleZoomValue;

  waCamEmuConstants_t constants;
  
  // bool thermalZoomApply;
  // bool visibleZoomApply;

}waCamEmu_t;

extern waCamEmu_t * const waCamEmu;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief  waCamEmu_Init
    @return none
*/
void waCamEmu_Init(void);

/** @brief  waCamEmu_Init
    @return none
*/
void waCamEmu_process(void);

#endif /*  */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

