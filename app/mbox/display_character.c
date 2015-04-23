#include "sys_include.h"
#include "ft_Font.h"

static char display_char[MAX_DISP_CHARACTER][MAX_DISP_CHAR] = {

    {"请连接好线缆，开始测试"},

    {"请输入网口工位号0~90:"},

    {"网络设置完毕，按回车键开始测试"},
    {"网络设置失败，不合格，测试结束"},

    {"网口测试成功"},
    {"网口测试失败"},

    {"串口测试成功"},
    {"串口测试失败"},

    {"硬盘测试成功"},
    {"硬盘测试失败"},

    {"tfcard测试成功"},
    {"tfcard测试失败"},

    {"请确认视频和音频输出"},
    {"输入媒体板信息，按回车键继续"},

    {"看串口上是否有输出，按回车键继续"},

    {"usb测试成功"},
    {"usb测试失败"},

    {"串口测试没有通过"},
    {"串口测试已经通过"},

    {"请按GPIO"},
    {"GPIO已经测试"},

    {"请不要对mic说话"}

};

int display_character_into_screen(DISP_CHARACTER type,
        Pixel64 FT,
        GrPos *pPos, GrRegion *pRegion)
{

    ft_Font_Str2Disp_return_region(&(display_char[type][0]),
        FT,
        *pPos,
        35,
        pRegion);

    return 0;
}



