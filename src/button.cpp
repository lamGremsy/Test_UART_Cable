/** 
  ******************************************************************************
  * @file    button.c
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    ${date}
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) ${year} Gremsy.  
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/

#include "button.h"
#include <wiringPi.h>
#include "stdbool.h"
/* Private typedef -----------------------------------------------------------*/
/// dinh nghia kiem bien trang thai xu ly cho button
typedef enum _buttonProcessState_t
{
    BUTTON_PROCESS_STATE_IDLE,
    BUTTON_PROCESS_STATE_WAIT,
    BUTTON_PROCESS_STATE_CHECK,
    BUTTON_PROCESS_STATE_HOLD,
    BUTTON_PROCESS_STATE_HOLD_WAIT,
    BUTTON_PROCESS_STATE_NUM,
    BUTTON_PROCESS_STATE_DONE,
    BUTTON_PROCESS_STATE_ERROR,
}buttonProcessState_t;

/// struct chua cac bien chay an cua thu vien button
typedef struct _buttonPrivate_t
{
    /// trang thai hoat dong cua button
	buttonProcessState_t state;

    /// bien luu thoi gian xu ly led
    int time_loop;

    /// flag press
    bool f_press;

    /// count press
    unsigned short count;

    /// thoi gian nhan
    int time_press;

    /// thoi gian khong nhan
    int time_release;

    /// muc hoat dong
    bool active_level;

    int time_hold;
    int time_rel;
    int time_noise;

}buttonPrivate_t;
/* Private define ------------------------------------------------------------*/
#define BUTTON_PIN      25
#define GPIO_PIN_SET    1
#define GPIO_PIN_RESET  0
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
buttonPrivate_t buttonPri;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/** @brief      button_init
 *  @param[in]
	@return
*/
void button_init(void)
{
  wiringPiSetup();

  // Sets the pin as input.
  pinMode(BUTTON_PIN,INPUT);
  // Sets the Pull-up mode for the pin.
  pullUpDnControl(BUTTON_PIN, PUD_UP);

  buttonPri.time_hold 	= 500;
  buttonPri.time_rel 	= 500;
  buttonPri.time_noise 	= 50;
}

/** @brief		buttonGet
 *  param[in]	none
	@return		GPIO_PinState
*/
static int buttonGet(void)
{
	return (digitalRead(BUTTON_PIN));
}

/** @brief      timeReset
 *  @param[in]
	@return
*/
static void timeReset(int *time)
{
  *time = millis();
}

/** @brief      timeGetMs
 *  @param[in]
	@return
*/
static int timeGetMs(int *time)
{
  return(millis() - *time);
}

/** @brief		buttonGetCount
 *  param[in]	none
	@return		tuple_t
*/
tuple_t buttonGetCount(void)
{
	return (tuple_t)buttonPri.count;
}

/** @brief		buttonTuplesGet
 *  param[in]	button : pointer address of struct button
	@return		tuple_t
*/
static tuple_t buttonTuplesGet(buttonPrivate_t *button)
{
	int timeHold 		= button->time_hold;
	int timeRelease 	= button->time_rel;

	tuple_t ret = EMPTY;

	if(button->state == BUTTON_PROCESS_STATE_IDLE)
	{
		/// default button param
		button->count = 0;
		button->f_press = false;


		/// reset time button press
		timeReset(&button->time_press);

		/// reset time button unPress
		timeReset(&button->time_release);

		/// next state
		button->state = BUTTON_PROCESS_STATE_WAIT;

	}
	else if(button->state == BUTTON_PROCESS_STATE_WAIT)
	{
		/// check button press
		if(buttonGet() == GPIO_PIN_RESET)
		{
			button->f_press = true;
		}
		else
		{
			/// check button before have press ? count up variables button count
			if(button->f_press == true)
			{
				button->f_press = false;

				/// count up variables button count
				button->count = 1; /// ??????????????????????

				/// reset time button press
				timeReset(&button->time_press);

				/// reset time button unPress
				timeReset(&button->time_release);

				/// next state
				button->state = BUTTON_PROCESS_STATE_CHECK;
			}
		}

		/// in case button press check button press ?
		if(button->f_press == true)
		{
			/// check button press ?
			if(timeGetMs(&button->time_press) > timeHold)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_HOLD;
			}
		}
		else
		{
			/// reset time button press
			timeReset(&button->time_press);
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_CHECK)
	{
		/// check button press ?
		if(buttonGet() == GPIO_PIN_RESET)
		{
			button->f_press = true;
		}
		else
		{
			/// in case button press check button press ?
			if(button->f_press == true)
			{
				button->f_press = false;

				/// count up button variables count
				button->count += 1;
			}
		}

		/// check button Error where press any time
		if(button->count > DEC)
		{
			button->state = BUTTON_PROCESS_STATE_ERROR;
		}

		/// kiem tra nut nhan con giu hay khong
		if(button->f_press == true)
		{
			/// check time button hold
			if(timeGetMs(&button->time_press) > timeHold)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_HOLD;
			}


			/// rest time button release
			timeReset(&button->time_release);
		}
		else
		{
			/// kiem tra nut nhan co duoc giu hay khong
			if(timeGetMs(&button->time_release) > timeRelease)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_NUM;
			}

			/// reset time button press
			timeReset(&button->time_press);
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_HOLD)
	{
		ret = HOLD;
		button->count = HOLD;

		/// next state
		button->state = BUTTON_PROCESS_STATE_HOLD_WAIT;
	}
	else if(button->state == BUTTON_PROCESS_STATE_HOLD_WAIT)
	{
		/// check button to exit state HOLD
		if(buttonGet() == GPIO_PIN_SET)
		{
			/// next state
			button->state = BUTTON_PROCESS_STATE_DONE;
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_NUM)
	{
		/// get button count
		ret = (tuple_t)button->count;

		/// next state
		button->state = BUTTON_PROCESS_STATE_DONE;
	}
	else if(button->state == BUTTON_PROCESS_STATE_DONE)
	{
		/// next state
		button->state = BUTTON_PROCESS_STATE_IDLE;
	}
	else if(button->state == BUTTON_PROCESS_STATE_ERROR)
	{
		/// next state
		button->state = BUTTON_PROCESS_STATE_IDLE;
	}

	return ret;
}

/** @brief		buttonPressProcess
 *  param[in]	button : pointer address of struct button
	@return		none
*/
static void buttonPressProcess(buttonPrivate_t *button)
{
	int timeNoise = button->time_noise;

	if(timeGetMs(&button->time_loop) >= timeNoise)
	{
		timeReset(&button->time_loop);
		buttonTuplesGet(button);
	}
}

/** @brief		buttonProcess
 *  param[in]	none
	@return		none
*/
void buttonProcess(void)
{
	buttonPressProcess(&buttonPri);
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/