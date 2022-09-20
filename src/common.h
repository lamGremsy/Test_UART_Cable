#include <cstring>
#include "serial_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#define DEBUG 1

#if (DEBUG == 1)
# define DebugMsg(fmt, args ...)        do {printf("[%s]:[%d][Msg]: " fmt , __FUNCTION__, __LINE__, ## args); } while(0);
# define DebugInfo(fmt, args ...)       do {printf("[%s]:[%d][Info]: " fmt "\n", __FUNCTION__, __LINE__, ## args); } while(0);
# define DebugWarning(fmt, args ...)    do {printf("[%s]:[%d][Warn]: " fmt "\n", __FUNCTION__, __LINE__, ## args); } while(0);
# define DebugError(fmt, args ...)      do {printf("[%s]:[%d][Error]: " fmt "\n", __FUNCTION__, __LINE__, ## args); } while(0);
#else
# define DebugMsg(fmt, args ...)
# define DebugInfo(fmt, args ...)
# define DebugWarning(fmt, args ...)
# define DebugError(fmt, args ...)
#endif

/*
	- Not alarm
	- Not  Get / Sets atmospheric temperature
	- Not CWSI graph ratios
	- Not Get / Set CWSI cross value 
	- Hold CWSI center value
*/

