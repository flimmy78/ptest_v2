#include "sys_include.h"
#include "i2c_public.h"
#include "temperature.h"

#define TEMPERATURE_I2C_ADDR 0x90
#define I2C_DEVICE_NUM 0
#define SHIFT(_a, _b) (1.0 * (((_a) >> (_b)) & 0x01))

float read_temperature(void)
{
    int ret;

    unsigned int u32DeviceAddress = TEMPERATURE_I2C_ADDR;
    unsigned int u32I2cNum  = I2C_DEVICE_NUM;
    unsigned int u32RegAddr = 0;
    unsigned int u32RegAddrCount = 1;
    unsigned int u32ReadNumber = 2;
    unsigned char *pData = NULL;
    unsigned short sdata, stmperature;
    float temp;

    ret = init_i2c_module();
    if (ret <= 0)
    {
        M_TRACE(ERROR_TRACE, SYS_MODE, "open i2c module failure\n");
        return -256;
    }

    pData =(unsigned char *) &(sdata);
    ret = i2c_read(u32I2cNum, u32DeviceAddress,
               u32RegAddr, u32RegAddrCount,
               pData, u32ReadNumber);
    if (ret <= 0)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "read i2c failure\n");
        return -256;
    }

    close_i2c_module();

    stmperature = pData[0];
	stmperature = (stmperature<< 8) + pData[1];
	temp =  SHIFT(stmperature, 4) / 16 +
			SHIFT(stmperature, 5) / 8 +
			SHIFT(stmperature, 6) / 4 +
			SHIFT(stmperature, 7) / 2 +
			SHIFT(stmperature, 8) +
			SHIFT(stmperature, 9) * 2 +
			SHIFT(stmperature, 10) * 4 +
			SHIFT(stmperature, 11) * 8 +
			SHIFT(stmperature, 12) * 16 +
			SHIFT(stmperature, 13) * 32 +
			SHIFT(stmperature, 14) * 64;

	if (stmperature & 0x8000)
	{
        temp = -1.0 * temp;
	}

    //M_TRACE(DEBUG_TRACE, SYS_MODE, "%3.4f\n", temp);

    return temp;
}

