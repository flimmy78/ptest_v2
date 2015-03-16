
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hi_unf_i2c.h"

static int bI2C_Enable = 0;

int init_i2c_module(void)
{
    HI_S32 s32Ret = HI_FAILURE;
    if (bI2C_Enable == 1)
    {
        bI2C_Enable = 0;
        return 0;
    }

    s32Ret = HI_UNF_I2C_Init();
    if (HI_SUCCESS != s32Ret)
    {
        return 0;
    }
    bI2C_Enable = 1;
    return 1;
}

int close_i2c_module(void)
{
    bI2C_Enable = 0;
    HI_UNF_I2C_DeInit();
    return 0;
}

int i2c_read(unsigned int u32I2cNum, unsigned int u32DeviceAddress,
        unsigned int u32RegAddr, unsigned int u32RegAddrCount,
        unsigned char *pData, unsigned int u32ReadNumber)
{

    HI_S32 s32Ret = HI_FAILURE;

    if (bI2C_Enable == 0)
    {
        return 0;
    }

    /* Read data from Device */
    s32Ret = HI_UNF_I2C_Read(u32I2cNum, u32DeviceAddress,
                 u32RegAddr, u32RegAddrCount,
                 pData, u32ReadNumber);
    if (s32Ret != HI_SUCCESS)
    {
        return 0;
    }

    return 1;
}

int i2c_write(unsigned int u32I2cNum, unsigned int u32DeviceAddress,
        unsigned int u32RegAddr, unsigned int u32RegAddrCount,
        unsigned char *pData, unsigned int u32WriteNumber)
{

    HI_S32 s32Ret = HI_FAILURE;

    if (bI2C_Enable == 0)
    {
        return 0;
    }

    s32Ret = HI_UNF_I2C_Write(u32I2cNum, u32DeviceAddress,
                 u32RegAddr, u32RegAddrCount,
                 pData, u32WriteNumber);
    if (s32Ret != HI_SUCCESS)
    {
        return 0;
    }

    return 1;
}

