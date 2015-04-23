#include "sys_include.h"
#include "i2c_public.h"
#include "rtc_public.h"

#define RTC_I2C_ADDR_WR 0xd0
#define RTC_I2C_ADDR_RE 0xd1

static void read_rtc_and_config_date(unsigned char *pData)
{
	unsigned char sec_data = 0, sec = 0, min_data = 0, min = 0, hour_data = 0, hour = 0;
	unsigned char day = 0, date_data = 0, date = 0, month_data = 0, month = 0, year_data = 0, year = 0;
	char Cmd[128] = {0};
	char fen = '"';

	sec_data = pData[0];
	min_data = pData[1];
	hour_data = pData[2];
	day = pData[3];
	date_data = pData[4];
	month_data = pData[5];
	year_data = pData[6];

	sec = ((sec_data >> 4) & 0x7) * 10 + (sec_data & 0xf);
	min = ((min_data >> 4) & 0x7) * 10 + (min_data & 0xf);
	hour = ((hour_data >> 4) & 0x03) * 10 + (hour_data & 0xf);
	date = ((date_data >> 4) & 0x3) * 10 + (date_data & 0xf);
	month = ((month_data >> 4) & 0x01) * 10 + (month_data & 0xf);
	year = ((year_data >> 4) & 0xf) * 10 + (year_data & 0xf);

	memset(Cmd, 0, 128);

    sprintf(Cmd, "date %c%04d-%02d-%02d %02d:%02d:%02d%c", fen, year+2000, month, date,
        	hour, min, sec, fen);

	system(Cmd);

    return ;
}

int get_time_info_string(char *pdata)
{
    time_t timep = 0;
    struct tm *p;

    unsigned char second_data, minute_data, hour_data;
    unsigned char day, date_data, month_data;
    int year_data;

    int ret;

    time(&timep);
    p = localtime(&timep);

    p->tm_mon += 1;
    second_data = p->tm_sec;
    minute_data = p->tm_min;
    hour_data = p->tm_hour;

    date_data  = p->tm_mday;
    month_data = p->tm_mon;
    year_data  = p->tm_year+1900;


    printf("for test\r\n");

    sprintf(pdata, "%4d-%02d-%02d %02d:%02d:%02d", year_data, month_data, date_data, hour_data, minute_data, second_data);

    if (year_data < 2014)
    {
        return 0;
    }

    return 1;
}

int save_time_into_rtc(void)
{
    time_t timep = 0;
    struct tm *p;

    unsigned char second_data, minute_data, hour_data;
    unsigned char day, date_data, month_data, year_data;
    unsigned char data[10];

    int ret;

    unsigned int u32DeviceAddress = RTC_I2C_ADDR_WR;
    unsigned int u32I2cNum  = 0;
    unsigned int u32RegAddr = 0x0;
    unsigned int u32RegAddrCount = 1;
    unsigned int u32WriteNumber = 7;

    time(&timep);
    p = localtime(&timep);

    p->tm_mon += 1;
    second_data = (((p->tm_sec / 10) & 0x07) << 4) | (p->tm_sec - (p->tm_sec / 10) * 10);

    minute_data= (((p->tm_min / 10) & 0x07) << 4) | (p->tm_min - (p->tm_min / 10) * 10);
    if ((p->tm_hour/10) >= 2)
    {
        hour_data = 0x20 | ((p->tm_hour - 20) & 0x0f);
    }
    else if ((p->tm_hour/10) >= 1)
    {
        hour_data = 0x10 | ((p->tm_hour - 10) & 0x0f);
    }
    else
    {
        hour_data = 0x00 | ((p->tm_hour) & 0x0f);
    }

    hour_data = hour_data & 0xBF;
    day = p->tm_wday;
    date_data = (((p->tm_mday / 10) & 0x03) << 4) | (p->tm_mday - (p->tm_mday / 10) * 10);
    month_data= (((p->tm_mon / 10) & 0x01) << 4) | (p->tm_mon - (p->tm_mon / 10) * 10);
    year_data= (1900 + p->tm_year) - 2000;
    year_data = (((year_data / 10) & 0x0F) << 4) | (year_data - (year_data / 10) * 10);

    memset(data, 0, 10);
    data[0] = second_data;
    data[1] = minute_data;
    data[2] = hour_data;
    data[3] = day;
    data[4] = date_data;
    data[5] = month_data;
    data[6] = year_data;

    ret = init_i2c_module();
    if (ret == 0)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "i2c init failure\n");
        return 0;
    }

    ret = i2c_write(u32I2cNum, u32DeviceAddress,
                u32RegAddr, u32RegAddrCount,
                data, u32WriteNumber);
    if (ret == 0)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE,"i2c write failure\n");
        close_i2c_module();
        return 0;
    }

    close_i2c_module();
    return 1;
}

int sync_time_from_rtc(void)
{
    int ret;
    unsigned int u32DeviceAddress = RTC_I2C_ADDR_RE;
    unsigned int u32I2cNum  = 0;
    unsigned int u32RegAddr = 0x0;
    unsigned int u32RegAddrCount = 1;
    unsigned int u32ReadNumber = 7;
    unsigned char data[10];

    memset(data, 0, 10);

    ret = init_i2c_module();
    if (ret == 0)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "init i2c failure\n");
        return 0;
    }

    ret = i2c_read(u32I2cNum, u32DeviceAddress,
                u32RegAddr, u32RegAddrCount,
                data, u32ReadNumber);
    if (ret == 0)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "read i2c data failure\n");
        close_i2c_module();
        return 0;
    }

    //read_rtc_and_config_date(data);
    close_i2c_module();

    return 1;
}


int get_current_time_string(char *pdata)
{
    time_t timep = 0;
    struct tm *p;

    unsigned char second_data, minute_data, hour_data;
    unsigned char day, date_data, month_data;
    int year_data;

    int ret;

    time(&timep);
    p = localtime(&timep);

    p->tm_mon += 1;
    second_data = p->tm_sec;
    minute_data = p->tm_min;
    hour_data = p->tm_hour;

    date_data  = p->tm_mday;
    month_data = p->tm_mon;
    year_data  = p->tm_year+1900;



    sprintf(pdata, "RTC|%4d-%02d-%02d %02d:%02d:%02d", year_data, month_data, date_data, hour_data, minute_data, second_data);
    printf("%s\n", pdata);
    return 1;
}

