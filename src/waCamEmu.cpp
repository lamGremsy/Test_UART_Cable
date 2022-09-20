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
#include "waCamEmu.h"
// #include "waCommandResponse.h"
/* Private typedef------------------------------------------------------------------------------*/

/* Private define------------------------------------------------------------------------------*/

#define MAX_NUMBER_OF_COMMAND 82

#define WA_CCP_HEADER_BYTE 0x02
#define WA_CCP_FOOTER_BYTE 0x03
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
static char waCamEmu_dataFream[256];

static waCamEmu_t s_waCamEmu = {0};
waCamEmu_t * const waCamEmu = &s_waCamEmu;

extern Serial_Port *serial_port_quit;

char wpCameraEmu_commandList[MAX_NUMBER_OF_COMMAND][20] = 
{
    {"HIWS\n"}, 
    {"GDLM\n"},
    {"GSRN\n"},
    {"AGGI TRUE\n"},
    {"IRCR"},
    {"ICPT"},
    {"GZTV"},//6
    {"GZVV"},
    {"GPTE"},
    {"GRMD"},
    {"GREN"},//10
    {"GALM"},
    {"GALC"},
    {"GALV"},
    {"GMST"},
    {"GMSI"},//15
    {"GMFR"},
    {"GTRT"},
    {"GTAT"},
    {"GTCP"},
    {"GTSC"},//20
    {"GCMA"},
    {"GCMI"},
    {"GCCE"},
    {"GCST"},
    {"GTUT"},
    {"GIRJ"},
    {"GIRT"},
    {"GISR"},
    {"GIVI"},
    {"GVTH"},//30

    {"GCWM"}, //  Get CWSI mode
    {"SCWM EMPIRICAL"}, //  Set CWSI mode  32
    {"SCWM THEORETIC"}, //  Set CWSI mode  33
    {"SCWM DIFFERENTIAL"},  //  Set CWSI mode 34
    {"GCAT"}, //  Get air temperature
    {"SCAT"}, //  Set air temperature
    {"GUSL"}, //  Get 100 % stress leve
    {"SUSL"}, //  Set 100 % stress leve
    {"GLSL"}, //  Get 0 % stress level
    {"SLSL"}, //  Set 0 % stress level
    {"GCRP"}, //  Get crop
    {"GCRP 1"}, //  Set crop
    {"GCHY"}, //  Get relative humidity
    {"SCHY"}, //  Set relative humidity
    {"GEDF"}, //  Get d1 // 45
    {"GEDS"}, //  Get d2 // 46
    {"GINB"}, //  Get intercept baseline
    {"SINB 1"}, //  Set intercept baseline
    {"GSLB"}, //  Get slope baseline
    {"SSLB 1"}, //  Set slope baseline
    {"GCWP"}, //  Get CWSI cross value
    {"GCGR"}, //  Get CWSI graph ratios
    {"GCWV"}, //  Get show center CWSI Value
    {"SCWV TRUE"}, //  Set show center CWSI Value
    {"GCMP"}, //  Get colourmap
    {"SCMP 0"}, // 56
    {"SCMP 1"},
    {"SCMP 2"},
    {"SCMP 3"},
    {"IMCR"}, // Triggers NUC
    {"SICT TRUE"}, //  Set CWSI TIFF image settings
    {"SISS TRUE"}, // Set screenshot JPEG settings
    {"GTII"},
    {"STII TRUE"},
    {"STII FALSE"},
    {"SLAY FULLSCREEN"}, // 66
    {"SLAY INSPECTION"}, // 67
    {"SLAY PIP"}, // 68
    {"SZIN\n"}, // 69
    {"SZOT\n"}, // 70
    {"SMCA THERMAL"}, // 71
    {"SMCA VISIBLE"}, // 72
    {"STUT C"},
    {"STUT F"},
    {"STUT K"},
    {"SCRP"},
    {"SZVN 0"},
    {"SZTN 0"},
    {"STTY"},
    {"GWIDGET"},
    {"SWIDGET"},
};

char wpCameraEmu_commandResponse[MAX_NUMBER_OF_COMMAND][70] = 
{
    {"OK"},
    {"LINE"},
    {"GREMSY-CAM-QC01"},
    {"OK"},
    {"FALSE"},
    {"FALSE"},
    {""},//{"0 1.000000"},//{"3 2.5"},
    {""},//{"0 1.000000"},//{"3 2.5"},
    {"ERROR"},//{"0 GRAY"},
    {"ERROR"},//{"AUTOMATIC"},
    {"ERROR"},//{"-25.0 150.0"},
    {"OFF"},
    {"RED BLUE GREEN"},
    {"20.0 40.0"},
    {"SSD READY\r\nFALSE_DRIVE N/A\r\nSD_CARD READY"},//{"READY"},
    {"SSD 123569995776\r\nFALSE_DRIVE N/A\r\nSD_CARD 31797542912"},//{"SSD 123456789"},
    {"SSD 90.520377\r\nFALSE_DRIVE N/A\r\nSD_CARD 98.964224"},//{"SSD 88.88"},
    {"ERROR"},//{"50.0"},
    {"ERROR"},//{"51.0"},
    {"120"},
    {"FALSE"},
    {"TRUE"},
    {"TRUE"},
    {"TRUE"},
    {"TRUE"},
    {"C"},//{"ERROR"},//
    {"FALSE"},
    {"FLASE"},
    {"FALSE"},
    {"FALSE"},
    {"TRUE"},

                               
    // {"EMPIRICAL"},// //  Get CWSI mode
    {"THEORETIC"},//  Get CWSI mode
    // {"DIFFERENTIAL"},//  Get CWSI mode
    {"OK"},
    {"OK"},
    {"OK"},
    {"20.0"}, //  Get air temperature
    {"OK"},
    {"25.5"}, //  Get 100 % stress leve
    {"OK"},
    {"25.0"}, //  Get 0 % stress level
    {"OK"},
    {"2"}, //  Get crop
    {"OK"},
    {"50.0"}, //  Get relative humidity
    {"OK"},
    {"1.40"}, //  Get d1
    {"-0.40"}, //  Get d2
    {"1.21"}, //  Get intercept baseline
    {"OK"},
    {"-1.21"}, //  Get slope baseline
    {"OK"},
    // {"MAXIMUM 30.12"}, //  Get CWSI cross value
    {"MAXIMUM OUT_OF_RANGE\r\nMINIMUM OUT_OF_RANGE\r\nCENTER OUT_OF_RANGE"}, //  Get CWSI cross value
    {"10.0 20.0 30.0 40.0 0.0"}, //  Get CWSI graph ratios
    {"TRUE"}, //  Get show center CWSI Value
    {"OK"},
    // {"CROP_MAP"}, //  Get colourmap
    {"WATER"}, //  Get colourmap
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"}, // Triggers NUC
    {"TRUE"}, //  Set CWSI TIFF image settings
    {"FALSE"}, // Set screenshot JPEG settings
    {"TRUE"}, // interpolation
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},//70
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"}, // 76
    {"OK"},
    {"OK"},
    {"OK"},
    {"OK"},
    {"WIDGET_01"}
};

char layoutNameDebug[4][20] = {"", "FULLSCREEN", "INSPECTION", "Pip"};
char mainSreenNameDebug[3][20] = {"", "THERMAL", "VISIBLE"};
const char cwsiMode[3][15] = {"EMPIRICAL", "THEORETIC", "DIFFERENTIAL"};

/* Private function prototypes------------------------------------------------------------------------------*/
/** @brief      waCamEmu_responseCommandGetZoom
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetZoom(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandSetCwsiMode
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetCwsiMode(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandSetCwsiMode
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetCwsiMode(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandGetConstantsValue
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetConstantsValue(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandSetLayout
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetLayout(char *responseArray, int commandIndex);

/** @brief      waCamEmu_responseCommandSetZoomInOut
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetZoomInOut(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandSetMainCamera
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetMainCamera(char *responseArray, int commandIndex);

/** @brief      waCamEmu_responseCommandGetWidget
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetWidget(char *responseArray, int commandListIndex);

/** @brief      waCamEmu_responseCommandSetWidget
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetWidget(char *responseArray, int commandListIndex);

static const waCamEmuResponseCommandHandleList_t responseCmdHandleList[] = 
{
    {6, waCamEmu_responseCommandGetZoom},
    {7, waCamEmu_responseCommandGetZoom},
    {31, waCamEmu_responseCommandGetCwsiMode},
    {32, waCamEmu_responseCommandSetCwsiMode},
    {33, waCamEmu_responseCommandSetCwsiMode},
    {34, waCamEmu_responseCommandSetCwsiMode},
    {45, waCamEmu_responseCommandGetConstantsValue},
    {46, waCamEmu_responseCommandGetConstantsValue},
    {66, waCamEmu_responseCommandSetLayout},
    {67, waCamEmu_responseCommandSetLayout},
    {68, waCamEmu_responseCommandSetLayout},
    {69, waCamEmu_responseCommandSetZoomInOut},
    {70, waCamEmu_responseCommandSetZoomInOut},
    {71, waCamEmu_responseCommandSetMainCamera},
    {72, waCamEmu_responseCommandSetMainCamera},
    {80, waCamEmu_responseCommandGetWidget},
    {81, waCamEmu_responseCommandSetWidget}
};
/* Private functions------------------------------------------------------------------------------*/

/** @group __WA_CAM_EMU_CONFIGURATION_
    @{
*/#ifndef __WA_CAM_EMU_CONFIGURATION_
#define __WA_CAM_EMU_CONFIGURATION_
/** @brief  waCamEmu_Init
    @return none
*/
void waCamEmu_Init(void)
{
    // waCommandResponse_Init();

    memset(waCamEmu_dataFream, 0, MAX_READ_DATA_SIZE);

    DebugInfo("Wiris Agro Camera Emulation Init");
}

#endif
/**
    @}
*/

/** @group __WA_CAM_EMU_FUNCTION_
    @{
*/#ifndef __WA_CAM_EMU_FUNCTION_
#define __WA_CAM_EMU_FUNCTION_
/** @brief      waCamEmu_crcCalculator
 *  @param[in]  *data : mang can tinh crc
 *  @param[in]  *len : chieu dai cua mang
    @return 
*/
static int waCamEmu_sendFream(char *data, int len)
{
    int result = 0;

    result = serial_port_quit->write_message(data, len);//waCamEmu->sendData(data, len);

    return result;
}

/** @brief      waCamEmu_crcCalculator
 *  @param[in]  *data : mang can tinh crc
 *  @param[in]  *len : chieu dai cua mang
    @return 
*/
static unsigned char waCamEmu_crcCalculator(char *data, int len)
{
    int i = 0;
    unsigned char crc = 0;
    bool console = false;

    for(i = 0; i < len; i++)
    {
        crc += data[i];
    }

    if(console == true) DebugWarning("crc = %d\n", crc);

    return crc;
}

/** @brief  
 *  @param[in]  *command : command can gui
 *  @param[in]  *len : chieu dai cua mang can gui (read)
    @return 
*/
static int waCamEmu_addDataToFream(char *command, int *len)
{
    int commandLength = 0;
    int freamLength = 0;
    int dataIndex = 0;
    bool console = false;

    memset(waCamEmu_dataFream, 0, MAX_READ_DATA_SIZE);

    waCamEmu_dataFream[0] = WA_CCP_HEADER_BYTE;

    /// check command
    if(command == NULL)
    {
        DebugError("command does not exist !!!");

        return -1;
    }

    /// get command len
    commandLength = strlen(command);

    freamLength += commandLength;

    waCamEmu_dataFream[1] = freamLength + 1;
    waCamEmu_dataFream[2] = (waCamEmu_dataFream[1] & 0x00ff) >> 8;

    /// copy data to fream
    while(dataIndex <= commandLength)
    {
        waCamEmu_dataFream[dataIndex + 3] = command[dataIndex];

        dataIndex++;
    }

    if(console == true) DebugInfo("dataIndex = %d", dataIndex);

    /// add \r char to fream
    waCamEmu_dataFream[freamLength + 3] = 0x0a;

    /// calculator crc
    waCamEmu_dataFream[3 + freamLength + 1] = waCamEmu_crcCalculator(waCamEmu_dataFream + 3, dataIndex + 1);

    *len  = 3 + freamLength + 1 + 1;

    waCamEmu_dataFream[*len] = WA_CCP_FOOTER_BYTE;

    return 1;
}

/** @brief      waCamEmu_findTrueCommand
 *  @param[in]  *data : mang can gui
    @return 
*/
static int waCamEmu_parseData(char *data, int freamLen, int *dataLen)
{
    char buffer[freamLen - 2] = {0};
    bool console = false;
    int i = 0;
    int dataLength = 0;

    /// copy data
    memcpy(buffer, waCamEmu->readBuffer + 1, freamLen - 1);

    memset(waCamEmu->readBuffer, 0, 256);

    if(console == true)
    {
        DebugMsg("");
        for(i = 0; i < freamLen - 2; i++)
        {
            printf("| 0x%x", buffer[i]);
        }

        printf("\n");
    }

    dataLength = buffer[0] | buffer[1] << 8;

    if(console == true) DebugInfo("dataLength = %d", dataLength);

    /// kiem tra data len
    if(dataLength != (freamLen - 2 - 3))
    {
        DebugError("fream len");

        return -1;
    }

    int crc = waCamEmu_crcCalculator(buffer + 2, dataLength);
    int crcRecv = buffer[dataLength + 2];
    /// check crc
    if(crc != crcRecv)
    {
        DebugError("fream crcRecv = %d", crcRecv);

        return -2;
    }

    /// cap vung nho cho con tro data
    // data = new char [dataLength];//calloc(dataLength, sizeof(char));

    /// copy data --> command data
    for(i = 0; i < dataLength; i++)
    {
        data[i] = buffer[i + 2];
    }

    *dataLen = dataLength;

    return 1;
}

/** @brief      waCamEmu_findTrueCommand
 *  @param[in]  *data : mang can gui
    @return 
*/
static int waCamEmu_findTrueCommand(char *command)
{
    int i = 0;
    int len = 0;
    bool console = false;

    for(i = 0; i < MAX_NUMBER_OF_COMMAND; i++)
    {
        if(memcmp(wpCameraEmu_commandList[i], command, strlen(wpCameraEmu_commandList[i])) == 0)
        {
            DebugInfo("____________________________found command %s [%d] _____________________________", wpCameraEmu_commandList[i], i);

            return i;
        }
    }

    char response[5] = {'E', 'R', 'R', 'O', 'R'};//{'O', 'K'};

    waCamEmu_addDataToFream(response, &len);

    if(console == true)
    {
        DebugMsg("");
        for(i = 0; i < len; i++)
        {
            printf("| 0x%x", waCamEmu_dataFream[i]);
        }
        printf("\n");
    }

    int result = waCamEmu_sendFream(waCamEmu_dataFream, len + 1);

    DebugWarning("command %s is not support auto send OK | send len = %d", command, result);

    if(console == true)
    {
        DebugInfo("send result = %d", result);

        printf("\n\n\n");
    } 

    return -1;
}

/** @brief      waCamEmu_findTrueCommand
 *  @param[in]  *data : mang can gui
    @return 
*/
static int waCamEmu_recieverFream(int *len)
{
    int readResult = 0;
    static int oldFrameSize = 0;
    int i = 0;
    bool console = true;
    char readBuffer[256] = {0};

    readResult = serial_port_quit->read_message(readBuffer, MAX_READ_DATA_SIZE);//waCamEmu->readData(waCamEmu->readBuffer, MAX_READ_DATA_SIZE)

    if(readResult > 0)
    {
        memcpy(waCamEmu->readBuffer + oldFrameSize, readBuffer, readResult);

        if(console == true)
        {
            DebugInfo("number of bytes reciever = %d", readResult);
            for(i = 0; i < readResult; i++)
            {
                printf("| 0x%x", readBuffer[i]);
            }

            printf("\n");
        }

        char headerByte = 0;
        i = 0;

        /// kiem tra header byte
        while(i < readResult)
        {
            headerByte = waCamEmu->readBuffer[i];
            
            if(headerByte == WA_CCP_HEADER_BYTE)
            {
                if(console == true) DebugInfo("headerByte pos = %d", i);

                break;
            }

            i++;
        }

        /// get number of bytes reciever
        oldFrameSize += readResult;

        /// kiem tra so phan tu nhan
        if(oldFrameSize < 3)
        {
            DebugError("data not enough to frame len calculator");

            printf("\n\n");

            return 0;
        }

        if(console == true)
        {
            DebugInfo("number of bytes in buffer = %d", oldFrameSize);

            for(i = 0; i < oldFrameSize; i++)
            {
                printf("| 0x%x", waCamEmu->readBuffer[i]);
            }

            printf("\n");
        }

        /// frame calculator from protocol
        int lenOfFrame = 3 + ((waCamEmu->readBuffer[2] << 8) | waCamEmu->readBuffer[1] ) + 2;

        if(lenOfFrame > oldFrameSize)
        {
            if(console == true)
            {
                DebugError("frame size not enough [lenOfFrame = %d | oldFrameSize = %d]", lenOfFrame, oldFrameSize);

                printf("\n\n");
            }

            return 0;
        }

        char footerByte = waCamEmu->readBuffer[lenOfFrame - 1];

        /// kiem tra fream
        if(footerByte == WA_CCP_FOOTER_BYTE)
        {
            *len = lenOfFrame;

            oldFrameSize = 0;

            return 1;           
        }
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetZoomInOut
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetZoomInOut(char *responseArray, int commandListIndex)
{
 
    int commandNumber = -1;
    bool isConsole = true;
    bool applyZoomValue = false;

    if(commandListIndex == 69)
    {
        commandNumber = 0;
        applyZoomValue = true;
    }
    else if(commandListIndex == 70)
    {
        commandNumber = 1;
        applyZoomValue = true;
    }

    if(commandNumber == 0)
    {
        if(waCamEmu->mainScreen == MAIN_CAMERA_THERMAL)
        waCamEmu->thermalZoomValue+=0.5;

        if(waCamEmu->mainScreen == MAIN_CAMERA_VISIBLE)
        waCamEmu->visibleZoomValue++;
    }
    else if(commandNumber == 1)
    {
        if(waCamEmu->mainScreen == MAIN_CAMERA_THERMAL)
        waCamEmu->thermalZoomValue-=0.5;

        if(waCamEmu->mainScreen == MAIN_CAMERA_VISIBLE)
        waCamEmu->visibleZoomValue--;

        /// limit zoom value
        if(waCamEmu->thermalZoomValue <= 0)
        {
            waCamEmu->thermalZoomValue = 0.0f;
        }

        /// limit zoom value
        if(waCamEmu->visibleZoomValue <= 0)
        {
            waCamEmu->visibleZoomValue = 0;
        }
    }

    if(applyZoomValue == true)
    {
        responseArray[0] = 'O';
        responseArray[1] = 'K';      

        return 1;
    }
    else
    {
        DebugError("_______ not found command zoom in/out ______");
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetZoomValue
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetZoomValueByIndex(char *responseArray, char *commandRecv)
{
    int commandRecvLength = strlen(commandRecv);
    bool isValue = false;
    char valueArray[MAX_READ_DATA_SIZE];
    int valueCount = 0;
    bool applyMainCamera = false;
    bool isThermalZoom = false;

    bool isConsole = true;

    /// find layout main camera index
    while(commandRecvLength >= 0)
    {
        if(isValue == false)
        {
            if(commandRecv[commandRecvLength] == 0x20)
            {
                isValue = true;

                applyMainCamera = true;

                if(isConsole == true) DebugInfo("__________got value set zoom by index___________\n\n");
            }
            else
            {
                if(    commandRecv[0] == 'S' 
                    && commandRecv[1] == 'Z' 
                    && commandRecv[2] == 'T' 
                    && commandRecv[3] == 'N'
                )
                {
                    isThermalZoom = true;

                    if(isConsole == true) DebugInfo("__________got command zoom thermal by index___________\n\n");
                }
            }        
        }
        else
        {
            valueArray[valueCount] = commandRecv[commandRecvLength];

            if(isConsole == true)
            {
                DebugInfo("[%2d] - 0x%x - %c", valueCount, valueArray[valueCount], valueArray[valueCount]);
            }

            valueCount++;
        }

        commandRecvLength--;
    }

    /// kiem tra co value hay khong
    if(isValue != true)
    {
        return -1;
    }

    if(isThermalZoom == true)
    {
        waCamEmu->thermalZoomValue = atof(valueArray);
    }
    else
    {
        waCamEmu->visibleZoomValue = atof(valueArray);
    }

    if(applyMainCamera == true)
    {
        responseArray[0] = 'O';
        responseArray[1] = 'K';    

        return 1;  
    }
    else
    {
        DebugError("_______ not found command zoom by index ______");
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetMainCamera
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetMainCamera(char *responseArray, int commandIndex)
{
    char mainCamera[3][20];
    bool applyMainCamera = false;

    DebugInfo("Main Camera List :\n %s\n %s", mainCamera[0], mainCamera[1]);

    if(commandIndex == 71)
    {
        waCamEmu->mainScreen = MAIN_CAMERA_THERMAL;
        applyMainCamera = true;
    }
    else if(commandIndex == 72)
    {
        waCamEmu->mainScreen = MAIN_CAMERA_VISIBLE;
        applyMainCamera = true;
    }

    if(applyMainCamera == true)
    {
        /// check camera layout
        if(waCamEmu->layout == CAMERA_LAYOUT_FULLSCREEN)
        {
            if(waCamEmu->mainScreen == MAIN_CAMERA_THERMAL)
            {
                responseArray[0] = 'E';
                responseArray[1] = 'R'; 
                responseArray[2] = 'R';
                responseArray[3] = 'O'; 
                responseArray[4] = 'R';

                waCamEmu->mainScreen = MAIN_CAMERA_KNOW;

                return -1;
            }
        }

        responseArray[0] = 'O';
        responseArray[1] = 'K';  

        return 1;
           
    }
    else
    {
        DebugError("_______ not found command set main camera ______");
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetLayout
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetLayout(char *responseArray, int commandIndex)
{
    char layout[3][20];
    bool applyLayout = false;

    DebugInfo("Layout Camera List :\n %s\n %s\n %s", layout[0], layout[1], layout[2]);

    if(commandIndex == 66)
    {
        waCamEmu->layout = CAMERA_LAYOUT_FULLSCREEN;
        applyLayout = true;
    }
    else if(commandIndex == 67)
    {
        waCamEmu->layout = CAMERA_LAYOUT_INSPECTION;
        applyLayout = true;
    }
    else if(commandIndex == 68)
    {
        waCamEmu->layout = CAMERA_LAYOUT_PiP;
        applyLayout = true;
    }

    if(applyLayout == true)
    {
        responseArray[0] = 'O';
        responseArray[1] = 'K';  

        return 1;    
    }
    else
    {
        DebugError("_______ not found command set layout ______");
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandGetZoom
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetZoom(char *responseArray, int commandListIndex)
{
    int commandNumber = -1;
    bool isConsole = true;

    if(commandListIndex == 7)
    {
        commandNumber = 0;
    }

    if(commandListIndex == 6)
    {
        commandNumber = 1;
    }
    
    if(commandNumber != -1)
    {
        if(commandNumber == 0)
        {
            sprintf(responseArray, "%d %d", commandNumber, waCamEmu->visibleZoomValue);
        }
        else if(commandNumber == 1)
        {
            sprintf(responseArray, "%d %.5f", commandNumber, waCamEmu->thermalZoomValue);
        }

        if(isConsole == true) DebugInfo("%s", responseArray);

        return 1;
    }
    else if(commandNumber == -1)
    {
        DebugError("_______not found command get zoom value_______");

        return -1;
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetCwsiMode
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetCwsiMode(char *responseArray, int commandListIndex)
{
    if(commandListIndex == 32)
    {
        waCamEmu->cwsiMode = CWSI_MODE_EMPIRICAL;
    }
    else if(commandListIndex == 33)
    {
        waCamEmu->cwsiMode = CWSI_MODE_THEORETIC;
    }
    else if(commandListIndex == 34)
    {
        waCamEmu->cwsiMode = CWSI_MODE_DIFFERENTIAL;
    }

    responseArray[0] = 'O';
    responseArray[1] = 'K';  

    return 1;
}

/** @brief      waCamEmu_responseCommandSetCwsiMode
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetCwsiMode(char *responseArray, int commandListIndex)
{
    if(commandListIndex == 31)
    {
        sprintf(responseArray, "%s", cwsiMode[waCamEmu->cwsiMode]);

        return 1;
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandGetConstantsValue
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetConstantsValue(char *responseArray, int commandListIndex)
{
    if(commandListIndex == 45)
    {
        waCamEmu->constants.D1 += 0.25;
        sprintf(responseArray, "%.6f", waCamEmu->constants.D1);

        return 1;
    }
    else if(commandListIndex == 46)
    {
        waCamEmu->constants.D2 += 0.35;
        sprintf(responseArray, "%.6f", waCamEmu->constants.D2);

        return 1;
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandGetWidget
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandGetWidget(char *responseArray, int commandListIndex)
{
    if(commandListIndex == 80)
    {
        sprintf(responseArray, "WIDGET_%d", waCamEmu->widgetType);

        return 1;
    }

    return 0;
}

/** @brief      waCamEmu_responseCommandSetWidget
 *  @param[in]  
    @return 
*/
static int waCamEmu_responseCommandSetWidget(char *responseArray, int commandListIndex)
{
    static bool type = false;

    if(commandListIndex == 81)
    {
        if(type == false)
        {
            waCamEmu->widgetType = CAMERA_WIDGET_TYPE_01;
        }
        else
        {
            waCamEmu->widgetType = CAMERA_WIDGET_TYPE_02;
        }

        type = !type;

        sprintf(responseArray, "OK");

        return 1;
    }

    return 0;
}

/** @brief      waCamEmu_FindSpecialCommand
 *  @param[in]  
    @return 
*/
static int waCamEmu_FindSpecialCommand(int commandIndex)
{
    for(int i = 0; i < waCamEmu->totalResponseCommandNumber; i++)
    {
        if(commandIndex == responseCmdHandleList[i].index)
        {
            DebugInfo("found special command = %d", responseCmdHandleList[i].index);
            return i;
        }
    }

    return -1;
}

/** @brief      waCamEmu_findTrueCommand
 *  @param[in]  *data : mang can gui
    @return 
*/
static void waCamEmu_responseCommand(void)
{
    int recieverResult = 0;
    int checkFreamResult = 0;
    int findCommandResult = -1;
    int recieverLength = 0;
    int dataLength = 0;
    int sendFreamResult = 0;
    char data[MAX_READ_DATA_SIZE] = {0};

    bool console = false;
    bool specialCommand = false;
    int i = 0;

    recieverResult = waCamEmu_recieverFream(&recieverLength);

    if(recieverResult == 1)
    {
        checkFreamResult = waCamEmu_parseData(data, recieverLength, &dataLength);

        if(checkFreamResult == 1)
        {
            findCommandResult = waCamEmu_findTrueCommand(data);

            // delete [] data;

            if(findCommandResult != -1)
            {               
                int commandSendLength = 0;
                int sendLength = 0;
                int specialCmd = 0;

                specialCmd = waCamEmu_FindSpecialCommand(findCommandResult);

                if(specialCmd != -1)
                {
                    char maxResponseLength = 10;
                    char response[maxResponseLength];

                    memset(response, 0, maxResponseLength);

                    specialCommand = true;
                    int result = 0;

                    if(console == true)
                    {
                        for(int i = 0; i < strlen(data); i++)
                        {
                            DebugInfo("command recv [%2d] - 0x%x - %c", i, data[i], data[i]);
                        }
                    }

                    responseCmdHandleList[specialCmd].Function_int(response, findCommandResult);

                    waCamEmu_addDataToFream(response, &commandSendLength);
                }
                else
                {
                    waCamEmu_addDataToFream(wpCameraEmu_commandResponse[findCommandResult], &commandSendLength);
                
                    if(findCommandResult == 77)
                    {
                        waCamEmu->visibleZoomValue = 0;
                    }

                    if(findCommandResult == 78)
                    {
                        waCamEmu->thermalZoomValue = 0;
                    }
                }

                sendLength = commandSendLength + 1;

                // if(console == true)
                // {

                    DebugMsg("fream send len = %d \n", sendLength);

                    for(i = 0; i < sendLength; i++)
                    {
                        DebugInfo("[%2d] - 0x%x - %c", i, waCamEmu_dataFream[i], waCamEmu_dataFream[i]);
                    }

                    printf("\n");
                // }

                sendFreamResult = waCamEmu_sendFream(waCamEmu_dataFream, sendLength);

                if(console == true)
                {
                    DebugInfo("sendFreamResult = %d", sendFreamResult);

                    printf("\n\n\n");
                }

                DebugInfo("response command [special = %d] successfully !!!\n\n", specialCommand);
            }
        }
    }

    if(specialCommand == true)
    {
        printf("\n----------------------------------------------------------------\n");
        printf("                         CAMERA APPEARANCE                         \n");
        DebugInfo("\n\nlayout = %s\nmainScreen = %s\nthermalZoom = %.10f\nvisibleZoom = %d\n\n",
        layoutNameDebug[waCamEmu->layout], mainSreenNameDebug[waCamEmu->mainScreen], waCamEmu->thermalZoomValue, waCamEmu->visibleZoomValue
        );
        printf("\n----------------------------------------------------------------\n");

        printf("\n----------------------------------------------------------------\n");
        printf("                         CAMERA CWSI                         \n");
        DebugInfo("\n\ncwsi mode = %s",
        cwsiMode[waCamEmu->cwsiMode]);
        printf("\n----------------------------------------------------------------\n");

        printf("\n----------------------------------------------------------------\n");
        printf("                         CAMERA Constants                         \n");
        DebugInfo("\n\nD1 = %.6f | D2 = %.6f\n\n",
        waCamEmu->constants.D1,
        waCamEmu->constants.D2);
        printf("\n----------------------------------------------------------------\n");   

        printf("\n----------------------------------------------------------------\n");
        printf("                         CAMERA widget                         \n");
        DebugInfo("\n\nwidget type = %d\n\n",
        waCamEmu->widgetType);
        printf("\n----------------------------------------------------------------\n");                   
    }
}

#endif
/**
    @}
*/

/** @group __WA_CAM_EMU_PROCESS_
    @{
*/#ifndef __WA_CAM_EMU_PROCESS_
#define __WA_CAM_EMU_PROCESS_
/** @brief  waCamEmu_Init
    @return none
*/
void waCamEmu_process(void)
{
    waCamEmu->layout        = CAMERA_LAYOUT_INSPECTION;
    waCamEmu->mainScreen    = MAIN_CAMERA_THERMAL;

    waCamEmu->constants.D1 = 0.0f;
    waCamEmu->constants.D2 = -3.0f;

    waCamEmu->widgetType = CAMERA_WIDGET_TYPE_01;

    waCamEmu->totalResponseCommandNumber = sizeof(responseCmdHandleList) / sizeof(waCamEmuResponseCommandHandleList_t);

    DebugInfo("Have %d response commands", waCamEmu->totalResponseCommandNumber);

    while(1)
    {
        waCamEmu_responseCommand();
    }
}

#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/