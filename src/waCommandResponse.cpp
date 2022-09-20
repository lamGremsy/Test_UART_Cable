/**
  ******************************************************************************
  * @file    .c
  * @author  Gremsy Team
  * @version v100
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ************************************************************
  ******************
  * @par
  * COPYRIGHT NOTICE: (c) 2011 Gremsy.
  * All rights reserved.Firmware coding style V1.0.beta
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/
/* Includes------------------------------------------------------------------------------*/
#include "waCommandResponse.h"
#include "common.h"
/* Private typedef------------------------------------------------------------------------------*/
/* Private define------------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
static waCommandResponse_t s_CmdResponse;
waCommandResponse_t * const CmdResponse = &s_CmdResponse;

// static const waCommandResponseHandleList_t s_responseHandleList[] = 
// {

// };
static char colourmapList[4][20] = 
{
   "CROP_MAP",
   "CROPSTEP_MAP",
   "WATER_MAP",
   "WATERSTEP_MAP" 
};

static char CwsiModeList[3][20] = 
{
   "EMPIRICAL",
   "THEORETIC",
   "DIFFERENTIAL"

};
/* Private function prototypes------------------------------------------------------------------------------*/
/** @brief  GetRandom
    @return int
*/
static int GetRandom(int min,int max);
/* Private functions------------------------------------------------------------------------------*/

/** @group __WA_COMMAND_RESPONSE_CONFIGURATION_
    @{
*/#ifndef __WA_COMMAND_RESPONSE_CONFIGURATION_
#define __WA_COMMAND_RESPONSE_CONFIGURATION_
/** @brief  waCommandResponse_Init
    @return none
*/
void waCommandResponse_Init(void)
{
    CmdResponse->colourMap = (E_WirisAgro_colourmap)GetRandom((int)COLOURMAP_CROP_MAP, (int)COLOURMAP_TOTAL);
    DebugInfo("Colousmap : %s", colourmapList[(int)CmdResponse->colourMap]);

    CmdResponse->CWSImode = (WirisAgro_CWSImode_t)GetRandom((int)CWSI_MODE_EMPIRICAL, (int)CWSI_MODE_TOTAL);
    DebugInfo("CWSImode : %s", CwsiModeList[(int)CmdResponse->CWSImode]);

    CmdResponse->airTemperature = (float)GetRandom(25, 30);
    DebugInfo("airTemperature : %.1f", CmdResponse->airTemperature);

    CmdResponse->stressLevel_100 = (float)GetRandom(45, 50);
    DebugInfo("stressLevel_100 : %.1f", CmdResponse->stressLevel_100);

    CmdResponse->stressLevel_0 = (float)GetRandom(45, 50);
    DebugInfo("stressLevel_0 : %.1f", CmdResponse->stressLevel_0);

    CmdResponse->crop = (float)GetRandom(1, 3);
    DebugInfo("crop : %d", CmdResponse->crop);

    // CmdResponse->relativeHumidity = (float)GetRandom(25, 30);
    // DebugInfo("airTemperature : %.1f", CmdResponse->airTemperature);
}

#endif
/**
    @}
*/

/** @group __WA_COMMAND_RESPONSE_FUNCTION_
    @{
*/#ifndef __WA_COMMAND_RESPONSE_FUNCTION_
#define __WA_COMMAND_RESPONSE_FUNCTION_
/** @brief  GetRandom
    @return int
*/
static int GetRandom(int min,int max)
{
    return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

#endif
/**
    @}
*/


/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

