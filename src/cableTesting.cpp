/**
  ******************************************************************************
  * @file    waCamEmu.c
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
#include "cableTesting.h"
#include "button.h"
#include <wiringPi.h>
/* Private typedef------------------------------------------------------------------------------*/
typedef enum
{
    CABLE_TESTING_STATE_IDLE = 0x00,
    CABLE_TESTING_STATE_RUNNING,
    CABLE_TESTING_STATE_CHECK_RESULT,
    CABLE_TESTING_STATE_DONE,
    CABLE_TESTING_STATE_ERROR,
    CABLE_TESTING_TOTAL_STATE

}cableTesting_state_t;

typedef struct
{
    uint16_t len;
    uint8_t *buffer;
    int trueCount;
    int falseCount;
}readMsgHandler_t;

typedef struct _cableTesting_t
{
    /* data */
    readMsgHandler_t usb;
    readMsgHandler_t uart;

    int usbSendStringLength;
    int uartSendStringLength;

    bool startTesting;
    int count;

    cableTesting_state_t state;
}cableTesting_t;

/* Private define------------------------------------------------------------------------------*/
#define LED_STATUS  24
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
extern Serial_Port *UsbPort;
extern Serial_Port *UartPort;

cableTesting_t cable;

static bool s_buttonStart = false;

static char usbSendString[]  = "USB : 152-154 Street No. 02, Van Phuc Residential City, Hiep Binh Phuoc Ward, Thu Duc City, Ho Chi Minh City, Vietnam\n";
static char uartSendString[] = "UART : 152-154 Street No. 02, Van Phuc Residential City, Hiep Binh Phuoc Ward, Thu Duc City, Ho Chi Minh City, Vietnam\n";

/* Private function prototypes------------------------------------------------------------------------------*/

/* Private functions------------------------------------------------------------------------------*/

void cableTesting_init(void)
{
    cable.uart.len = 0;
    cable.uart.buffer = new uint8_t[MAX_READ_DATA_SIZE];
    if(cable.uart.buffer == NULL)
    {
        DebugInfo("uart read buffer malloc failed !!!");
    }

    memset(cable.uart.buffer, 0, MAX_READ_DATA_SIZE);

    cable.usb.len = 0;
    cable.usb.buffer = new uint8_t[MAX_READ_DATA_SIZE];
    if(cable.usb.buffer == NULL)
    {
        DebugInfo("usb read buffer malloc failed !!!");
    }
    memset(cable.usb.buffer, 0, MAX_READ_DATA_SIZE);

    cable.usbSendStringLength = strlen(usbSendString);
    cable.uartSendStringLength = strlen(uartSendString);

    DebugInfo("usbSendStringLength = %d", cable.usbSendStringLength);
    DebugInfo("uartSendStringLength = %d", cable.uartSendStringLength);
}

void cableTesting_SendDataProcess(void)
{
    uint8_t sendResult[2];

    for(;;)
    {
        if(cable.startTesting == true)
        {
            sendResult[0] = UsbPort->write_message(usbSendString, cable.usbSendStringLength);
            sendResult[1] = UartPort->write_message(uartSendString, cable.uartSendStringLength);

            DebugInfo("Send Data Count [%d][USB:%d][UART:%d]", cable.count++, sendResult[0], sendResult[1]);
        }

        usleep(10000);
    }
}

void cableTesting_UsbReadProcess(void)
{
    int readLength = 0;
    char readBuffer[MAX_READ_DATA_SIZE];

    for(;;)
    {
        readLength = 0;
        memset(readBuffer, 0, MAX_READ_DATA_SIZE);

        readLength = UsbPort->read_message(readBuffer, 256);
        if(readLength > 0)
        {
            // DebugInfo("\nreadLength = %d\n", readLength);

            // printf("\nData[");
            // for(int i = 0; i < readLength; i++)
            // {
            //     printf("0x%x", readBuffer[i]);
            // }
            // printf("]\n");

            /// get data
            memcpy(cable.usb.buffer + cable.usb.len, readBuffer, readLength);

            /// get next len
            cable.usb.len += readLength;

            /// check final byte
            if(readBuffer[readLength - 1] == '\n')
            {
                // DebugInfo("cable.usb.len = %d", cable.usb.len);
                // for(int i = 0; i < cable.usbSendStringLength; i++)
                // {
                //     printf("%c", readBuffer[i]);
                // }
                // printf("\n");

                if(cable.uartSendStringLength == cable.usb.len)
                {
                    if(memcmp(uartSendString, cable.usb.buffer, cable.usbSendStringLength) == 0)
                    {
                        cable.usb.trueCount++;
                        DebugInfo("usb find true String [%d]", cable.usb.trueCount);
                    }
                    else
                    {
                        cable.usb.falseCount++;
                        DebugInfo("usb NOT find true String [%d]", cable.usb.falseCount);
                    }
                }
                else
                {
                        cable.usb.falseCount++;
                        DebugInfo("usb recieve len ERROR !!!");
                        DebugInfo("usb NOT find true String [%d]", cable.usb.falseCount);
                }



                cable.usb.len = 0;
            }
        }
    }

}

void cableTesting_UartReadProcess(void)
{
    int readLength = 0;
    char readBuffer[256];

    DebugInfo("\nSTART UART READ THREAD !!!\n");

    for(;;)
    {
        readLength = 0;
        memset(readBuffer, 0, 256);

        readLength = UartPort->read_message(readBuffer, 256);
        if(readLength > 0)
        {
            // DebugInfo("\nreadLength = %d\n", readLength);

            // printf("\nData[");
            // for(int i = 0; i < readLength; i++)
            // {
            //     printf("0x%x", readBuffer[i]);
            // }
            // printf("]\n");

            /// get data
            memcpy(cable.uart.buffer + cable.uart.len, readBuffer, readLength);

            /// get next len
            cable.uart.len += readLength;

            /// check final byte
            if(readBuffer[readLength - 1] == '\n')
            {
                // DebugInfo("cable.uart.len = %d", cable.uart.len);
                // for(int i = 0; i < cable.usbSendStringLength; i++)
                // {
                //     printf("%c", readBuffer[i]);
                // }
                // printf("\n");

                if(cable.usbSendStringLength == cable.uart.len)
                {
                    if(memcmp(usbSendString, cable.uart.buffer, cable.uartSendStringLength) == 0)
                    {
                        cable.uart.trueCount++;
                        DebugInfo("uart find true String [%d]", cable.uart.trueCount);
                    }
                    else
                    {
                        cable.uart.falseCount++;
                        DebugInfo("uart NOT find true String [%d]", cable.uart.falseCount);
                    }
                }
                else
                {
                        cable.uart.falseCount++;
                        DebugInfo("uart recieve len ERROR !!!");
                        DebugInfo("uart NOT find true String [%d]", cable.uart.falseCount);
                }


               cable.uart.len = 0;
            }
        }
    }

}

void cableTesting_manager(void)
{

    static tuple_t Bpress = EMPTY;
    static int ledStatusRunningTime = 1000000;

	DebugInfo("START THREAD MANAGER !!!");

	button_init();

    pinMode(LED_STATUS , OUTPUT);

	DebugInfo("Press Button Once to start testing !!!");

    // Bpress = SINGLE;

    for(;;)
    {
        buttonProcess();

        Bpress = buttonGetCount();

        if(cable.state == CABLE_TESTING_STATE_IDLE)
        {
            if(Bpress == SINGLE && s_buttonStart == false)
            {
                s_buttonStart = true;

                cable.startTesting = true;

                cable.state = CABLE_TESTING_STATE_RUNNING;
            }
        }
        else if(cable.state == CABLE_TESTING_STATE_RUNNING)
        {
            if(cable.count >= 100 * 5) // 1s
            {
                cable.count = 0;

                cable.startTesting = false;

                cable.state = CABLE_TESTING_STATE_CHECK_RESULT;
            }
        }
        else if(cable.state == CABLE_TESTING_STATE_CHECK_RESULT)
        {
            cable.state = CABLE_TESTING_STATE_ERROR;

            /// check true count
            if(cable.uart.trueCount >= 100 && cable.usb.trueCount >= 100)
            {
                cable.state = CABLE_TESTING_STATE_DONE;
            }

            /// check false count
            if(cable.uart.falseCount > 0 || cable.usb.falseCount > 0)
            {
                cable.state = CABLE_TESTING_STATE_ERROR;
            }

            DebugInfo("Testing result [%d] [%d|%d] [%d|%d]"
            , cable.state
            , cable.uart.trueCount
            , cable.usb.trueCount
            , cable.uart.falseCount
            , cable.usb.falseCount);

            memset(&cable.uart, 0, sizeof(readMsgHandler_t));
            memset(&cable.usb, 0, sizeof(readMsgHandler_t));
        }
        else if(cable.state == CABLE_TESTING_STATE_DONE)
        {
            

            if(Bpress == DOUBLE && s_buttonStart == true)
            {
                Bpress = EMPTY;

                s_buttonStart = false;

                /// back to new section
                DebugInfo("DONE - back to new section");

                cable.state = CABLE_TESTING_STATE_IDLE;

            }
        }
        else if(cable.state == CABLE_TESTING_STATE_ERROR)
        {


            if(Bpress == DOUBLE && s_buttonStart == true)
            {
                Bpress = EMPTY;

                s_buttonStart = false;

                /// back to new section
                DebugInfo("ERROR - back to new section");

                cable.state = CABLE_TESTING_STATE_IDLE;

            }
        }
    }
}

void ledStatusToogle(bool *state)
{
    if(*state == true)
    {
        digitalWrite(LED_STATUS, HIGH);
    }
    else
    {
        digitalWrite(LED_STATUS, LOW);
    }

    *state = !*state;
}

void cableTesting_ledStatus(void)
{
    static int ledTime = 1000000;
    static bool ledState = false;

    for(;;)
    {
        if(cable.state == CABLE_TESTING_STATE_IDLE)
        {
            ledTime = 1000000;

            ledStatusToogle(&ledState);
        }

        if(cable.state == CABLE_TESTING_STATE_RUNNING)
        {
            ledTime = 100000;

            ledStatusToogle(&ledState);
        }

        if(cable.state == CABLE_TESTING_STATE_DONE)
        {
            digitalWrite(LED_STATUS, LOW);
        }

        if(cable.state == CABLE_TESTING_STATE_ERROR)
        {
            digitalWrite(LED_STATUS, HIGH);
        }
        

        usleep(ledTime);
    }
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/