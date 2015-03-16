/*! @file
********************************************************************************
<PRE>
模块名       : 调试输出模块定义头文件
文件名       : debugPublic.h
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

#ifndef _DEBUG_PUBLIC_H_
#define _DEBUG_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

//初始化调试输出模块
//这个函数必须提前调用
void tracePrintfInit(void);

//按照模块设置打印的级别例如:modifyTraceLevelByMode(FT_MODE, DEBUG_TRACE);
//设定FT_MODE模块的打印级别为调试级别
void modifyTraceLevelByMode(int mode, int printLevel);

//打印输出实体函数
void tracePrintf(const char * funcN, int lineNo, char * pucFormat, ...);

enum
{
    SYS_MODE,
    GEN_MODE,
    HOT_MODE,
    UPGRADE_MODE,
    FB_MODE,

    NET_MODE,
    XML_MODE,
    FILE_MODE,

    IMG_MODE,
    PAGE_MODE,
    FT_MODE,
    SUB_MODE,

    TRACE_CNT
};

int traceModLevel[TRACE_CNT];

#define DEBUG_TRACE  30
#define WARING_TRACE 20
#define ERROR_TRACE  10
#define PRINT_TRACE  0

//打印输出信息的最大字符串长度
#define DEBUG_INFO_BUFFER_LEN	1024

//调试信息打印输出
#define M_TRACE(lvl, mode, pucFormat...) 	\
         if(lvl <= traceModLevel[mode]){\
            tracePrintf(__FUNCTION__, __LINE__, ##pucFormat);}

#ifdef __cplusplus
}
#endif

#endif

