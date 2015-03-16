/*! @file
********************************************************************************
<PRE>
模块名       : 调试输出模块实现模块
文件名       : tracePrintf.c
相关文件     : 无
文件实现功能 : 实现按照模块，按照等级的信息打印输出功能
作者         : macros.zhang
版本         : 1.0
--------------------------------------------------------------------------------
备注         : 无
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容
2010/02/22   1.0      macros
</PRE>
*******************************************************************************/

#include <sys_include.h>
#include <stdarg.h>
#include <sys/time.h>
#include "debug_public.h"

static char buffer[DEBUG_INFO_BUFFER_LEN];

//初始化调试信息打印输出模块
//默认错误级别
void tracePrintfInit(void)
{
    int i = 0;

    for (i = 0; i < TRACE_CNT; i++)
    {
        traceModLevel[i] = ERROR_TRACE;
    }

    return ;
}

//按照模块设定调试级别
void modifyTraceLevelByMode(int mode, int printLevel)
{
    traceModLevel[mode] = printLevel;

    return ;
}

void tracePrintf(const char *funcN, int lineNo, char *pucFormat, ...)
{
    va_list         	va;
    struct timeval	tv;

    memset(buffer, 0, DEBUG_INFO_BUFFER_LEN);
    if(pucFormat != NULL)
    {
        va_start(va, pucFormat);
        vsprintf(buffer, pucFormat, va);
        va_end(va);

        gettimeofday(&tv, NULL);
        fprintf(stderr, "[%-8d:%-6d][%-15s][%-4d:] %s",	(int)(tv.tv_sec), (int)(tv.tv_usec), funcN, lineNo, buffer);
    }

    return ;
}

