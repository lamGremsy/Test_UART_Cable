/** 
  ******************************************************************************
  * @file    button.h
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    ${date}
  * @brief   This file contains all the functions prototypes for the button.c
  *          firmware library
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2018 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __BUTTON_
#define __BUTTON_

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/// dinh nghia kieu so dem la ma
typedef enum _tuple_t
{
    EMPTY = 0,
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
    QUAD   = 4,
    QUIN   = 5,
    HEXA   = 6,
    SEP    = 7,
    OCT    = 8,
    NON    = 9,
    DEC    = 10,
    HOLD   = 11,
}tuple_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief      button_init
 *  @param[in]
	@return
*/
void button_init(void);

/** @brief		buttonProcess
 *  param[in]	none
	@return		none
*/
void buttonProcess(void);

/** @brief		buttonGetCount
 *  param[in]	none
	@return		tuple_t
*/
tuple_t buttonGetCount(void);
#endif /* __BUTTON_ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
