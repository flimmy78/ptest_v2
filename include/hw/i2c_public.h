
#ifndef _I2C_PUBLIC_H_
#define _I2C_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

int init_i2c_module(void);
int close_i2c_module(void);

int i2c_read(unsigned int u32I2cNum, unsigned int u32DeviceAddress,
        unsigned int u32RegAddr, unsigned int u32RegAddrCount,
        unsigned char *pData, unsigned int u32ReadNumber);

int i2c_write(unsigned int u32I2cNum, unsigned int u32DeviceAddress,
        unsigned int u32RegAddr, unsigned int u32RegAddrCount,
        unsigned char *pData, unsigned int u32WriteNumber);

#ifdef __cplusplus
}
#endif

#endif

