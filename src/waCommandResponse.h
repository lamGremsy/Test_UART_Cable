/** 
  ******************************************************************************
  * @file    waCommandResponse.h
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

#ifndef __WACOMMANDRESPONSE_H
#define __WACOMMANDRESPONSE_H

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
/* Exported define ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  WA_COMMAND_RESPONSE_ERROR = -1,
  WA_COMMAND_RESPONSE_RUNNING = 0,
  WA_COMMAND_RESPONSE_SUCCESSFUL = 1,

}waCommandReturnCode_t;

/** @addtogroup WIRIS_AGRO_COLOURMAP_FUNCTIONS
  * @{
  */
typedef enum
{
    COLOURMAP_UNKNOW = 0x00,
    COLOURMAP_CROP_MAP = 0x01,
    COLOURMAP_CROPSTEP_MAP,
    COLOURMAP_WATER_MAP,
    COLOURMAP_WATERSTEP_MAP,
    COLOURMAP_TOTAL,
    
}E_WirisAgro_colourmap;

/**
  * @brief 
  * @details 
  * @note 
  */
typedef enum
{
    CWSI_MODE_UNKNOW = 0x00,
    CWSI_MODE_EMPIRICAL = 0x01,
    CWSI_MODE_THEORETIC,
    CWSI_MODE_DIFFERENTIAL,
    CWSI_MODE_TOTAL,
    
}WirisAgro_CWSImode_t;

// typedef waCommandReturnCode_t (*waCommandResponseCallback)(const char *data, int len);

// typedef struct
// {
//   char *valueStr;
//   waCommandReturnCode_t (*waCommandResponseCallback)(const char *data, int len);

// }waCommandResponseHandleList_t;

typedef struct 
{
  E_WirisAgro_colourmap colourMap;
  WirisAgro_CWSImode_t CWSImode;
  float airTemperature;
  float stressLevel_100;
  float stressLevel_0;
  uint8_t crop;
  float relativeHumidity;
  float d1Constant;
  float d2Constant;
  float interceptBaseLine;
  float slopeBaseLine;
  float CWSIcrossValue;
  float CWSIgraphRatios;

  bool isCWSIcenterValue;
  bool isTriggerUNC;
  bool isCWSITIFFimage;
  bool isCreenshotJPEGimage;

}waCommandResponse_t;
/* Exported constants --------------------------------------------------------*/
/* Private variables----------------------------------------------------------*/
extern waCommandResponse_t * const CmdResponse;
/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief  waCommandResponse_Init
    @return none
*/
void waCommandResponse_Init(void);

#endif /* __WACOMMANDRESPONSE_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

