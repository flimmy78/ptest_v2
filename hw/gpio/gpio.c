#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_gpio.h"
#include "hi_adp_boardcfg.h"

#include "gpio_public.h"
#include "debug_public.h"

int read_gpio_status(unsigned int gpio_num, int *pvalue)
{
    int ret = 0;
    HI_BOOL bBitVal;

    HI_SYS_Init();

    ret = HI_UNF_GPIO_Init();
    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "gpio init failure\n");
        goto err0;
    }

    //设置成输入
    ret = HI_UNF_GPIO_SetDirBit(gpio_num, HI_TRUE);
    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "set dir bit failure\n");
        goto err1;
    }

    ret = HI_UNF_GPIO_ReadBit(gpio_num, &bBitVal);
    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "read bit failure\n");
        goto err1;
    }

    *pvalue = bBitVal;

    ret = 1;

err1:
    ret = HI_UNF_GPIO_DeInit();

err0:
    HI_SYS_DeInit();

    return ret;
}

int write_gpio_status(unsigned int gpio_num, int value)
{
    int ret = 0;
    HI_SYS_Init();

    ret = HI_UNF_GPIO_Init();
    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "gpio init failure\n");
        goto err_0;
    }

    //设置成输出
    ret = HI_UNF_GPIO_SetDirBit(gpio_num, HI_FALSE);
    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "set dir bit failure\n");
        goto err_1;
    }

    if (value == 0)
    {
        ret = HI_UNF_GPIO_WriteBit(gpio_num, HI_FALSE);
    }
    else
    {
        ret = HI_UNF_GPIO_WriteBit(gpio_num, HI_TRUE);
    }

    if (HI_SUCCESS != ret)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "write bit failure\n");
        goto err_1;
    }
    ret = 1;

err_1:
    ret = HI_UNF_GPIO_DeInit();
err_0:
    HI_SYS_DeInit();

    return ret;

}


