#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "sal/serialcom.h"

#include <time.h>

static int fd_ser = -1;
static MSG msg;
static pthread_t ser_pro;
static int state = 0;
static int count =0;
static int bClose = 0;
static unsigned char info[256];

static void randKey(char *key, int len)
{
	char c0;
	char c1;
	int i = 0;

	c0 = key[0];
	c1 = key[1];
	for(i = 2; i < len; i += 2)
	{
		key[i-2] = key[i];
		key[i-1] = key[i+1];
	}
	key[len-2] = c0;
	key[len-1] = c1;

	c0 = key[0];
	key[0] = key[len -2];
	key[len -2] = c0;
}

static void send(MSG msg)
{
	int i;

	memset(info, 0, 256);
	memcpy(info, &msg, sizeof(MSG));

	if(fd_ser)
	{
		for(i = 0; i < sizeof(MSG); i++)
		{
			SerialSend(fd_ser, &(info[i]), 1);
#ifdef SJDF
			usleep(15000);
#else
			usleep(13000);
#endif
		}
		printf("\n");
	}
	count++;
	//printf("[%d]send ok!\n", count);
}
static void receive()
{
	int len, i;
	unsigned char info[256];

	memset(info, 0, 256);
	if(fd_ser)
	{
		len = serialReceive(fd_ser, info, 256);
		for(i = 0; i < len; i++)
			printf("%02x ", info[i]);
		printf("\n");
	}
}

void sendser_msg(void)
{
    send(msg);
}

static void *verificate()
{
	int i = 0, j = 0;
	clock_t start;
	clock_t end;
	while(bClose)
	{
		if(state)
		{

			i = 0;
//			usleep(1000*1000);
			while(i < 60 && state)
			{
				send(msg);
				randKey(msg.body, 16);
#ifdef SJDF
				usleep(4700*1000);
#else
				usleep(4800*1000); //1*1000代表1ms
#endif
				i += 5;
				//printf("current play time = [%d]\n", i);
			}
			while(state)
			{
#ifdef SJDF
				usleep(89000*1000);
#else
				usleep(89000*1000);
#endif
				send(msg);
				randKey(msg.body, 16);
				i += 90;
				//printf("current play time = [%d]\n", i);
			}
		}
		else
		{
			usleep(1000*1000);
		}
	}


	return ;
}
int openSerialModel()
{
	int ret;
	fd_ser = InitSerialCom(2,9600,'n',1, 8);
	if(!fd_ser)
		return -1;
	state = 0;
	memcpy(msg.syn, "00", 2);

	memcpy(msg.head, "DC1", 3);
#ifdef SJDF
	memcpy(msg.body, "123456789ABCDE40", 16);
#else
	memcpy(msg.body, "341278569ABC40DE", 16);
#endif
	msg.end = 0x0D;
	bClose = 1;
	ret = pthread_create(&ser_pro, NULL, verificate, NULL);
	if(ret)
	{
		printf("pthread_create error!\n");
	}
}
void startSerialModel()
{
    state = 1;
    count = 0;
}
void stopSerialModel()
{
	state = 0;
	count = 0;
	printf("stop serial port.\n");
}

void closeSerialModel(void)
{
	bClose = 0;
}

/*
高精度定时
输入需要等待的毫秒数
内部实现，每次等待剩余时间的90%
然后依据内部时间获取实际等待时间，并计算剩余时间
以此循环到剩余时间不超过10毫秒
最后考虑函数调用以及usleep函数的不精确性，
将误差控制在一毫秒以内
*/
void sleep_by_ms(long long ms)
{
	static struct timeval s_tv;
	static struct timeval c_tv;
	long long cost;
	long long resume;

	if(ms <= 0)
		return;
	cost = 0;
	resume = ms;
	while(resume > 10)
	{
		gettimeofday(&s_tv, NULL);
		usleep(resume*900);
		gettimeofday(&c_tv, NULL);
		cost += (c_tv.tv_sec -s_tv.tv_sec)*1000 + (c_tv.tv_usec - s_tv.tv_usec)/1000;
		resume = ms - cost;
	}
	if(resume > 1)
		usleep(resume*1000 - 1000);
	return;
}

