#include     <stdio.h>     	 	/*标准输入输出定义*/
#include     <stdlib.h>     		/*标准函数库定义*/
#include     <unistd.h>    	 	/*Unix 标准函数定义*/
#include     <fcntl.h>      		/*文件控制定义*/
#include     <termios.h>    		/*PPSIX 终端控制定义*/
#include     <errno.h>      		/*错误定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <sys/ioctl.h>

#include	   "uart.h"

#ifdef __cplusplus
extern "C"{
#endif
#define TTYS_PORT0       	"/dev/ttyAMA0"
#define TTYS_PORT1       	"/dev/ttyAMA1"
#define TTYS_PORT2       	"/dev/ttyAMA2"
#define TTYS_PORT3       	"/dev/ttyAMA3"

#define BAUD_RATE9600	B9600
#define BAUD_RATE19200	B19200
#define BAUD_RATE38400	B38400
#define BAUD_RATE57600	B57600
#define BAUD_RATE115200	B115200

#define READ_MAX_LEN	300
#define SEND_SIZE 		8

static int SerialProcState = 1;


/*=====================================================================*/

/*函数描述:设置串口波特率*/
static int SetSpeed(int baudrate,struct termios*opt)
{
	switch(baudrate)
	{
		case 9600:
			cfsetispeed (opt, BAUD_RATE9600);
			cfsetospeed(opt, BAUD_RATE9600);
			break;

		case 19200:
			cfsetispeed (opt, BAUD_RATE19200);
			cfsetospeed(opt, BAUD_RATE19200);
			break;

		case 38400:
			cfsetispeed (opt, BAUD_RATE38400);
			cfsetospeed(opt, BAUD_RATE38400);
			break;

		case 57600:
			cfsetispeed (opt, BAUD_RATE57600);
			cfsetospeed(opt, BAUD_RATE57600);
			break;

		case 115200:
			cfsetispeed (opt, BAUD_RATE115200);
			cfsetospeed(opt, BAUD_RATE115200);
			break;

		default:
			printf("set baud rate fail ,use the old setting");
			return -1;
	}
	return 0;
}

/*函数描述:设置串口的奇偶校验*/
static int SetParity(char Parity, struct termios* opt)
{
	switch( Parity )
	{
		case 'O': //奇数
		case 'o':
			opt->c_cflag |= PARENB;
			opt->c_cflag |= PARODD;
			opt->c_iflag |= (INPCK | ISTRIP);
			break;

		case 'E': //偶数
		case 'e':
			opt->c_iflag |= (INPCK | ISTRIP);
			opt->c_cflag |= PARENB;
			opt->c_cflag &= ~PARODD;
			break;

		case 'N': //无奇偶校验位
		case 'n':
			opt->c_cflag &= ~PARENB;
			break;

	default : //无奇偶校验位
		opt->c_cflag &= ~PARENB;
		break;
	}
}
/*函数描述:设置串口的停止位*/
static int SetStop(char nStop, struct termios* opt)
{
	if( nStop != 2 )
	{
		opt->c_cflag &= ~CSTOPB; //1个停止位
	}
	else
	{
		opt->c_cflag |= CSTOPB;//2个停止位
	}

	return 0;

}
/*函数描述:设置串口的数据位*/
static int SetDatanum(char datanum, struct termios* opt)
{
	switch(datanum)
	{
		case 6:
			opt->c_cflag |= CS6;
			break;

		case 7:
			opt->c_cflag |= CS7;
			break;

		case 8:
			opt->c_cflag |= CS8;
			break;

		default:
			opt->c_cflag |= CS8;
			break;
	}
	return 0;
}

/* send verify data*/
void sendVerifyData(int fd)
{
	char msg[24];
	memset(msg, 0, 24);
	msg[0] = 0x30;
	msg[1] = 0x30;
	msg[2] = 0x44;
	msg[3] = 0x43;
	msg[4] = 0x31;

	msg[5] = 0x31;	msg[6] = 0x32;
	msg[7] = 0x33;	msg[8] = 0x34;
	msg[9] = 0x35;	msg[10] = 0x36;
	msg[11] = 0x37;	msg[12] = 0x38;
	msg[13] = 0x39;	msg[14] = 0x41;
	msg[15] = 0x42;	msg[16] = 0x43;
	msg[17] = 0x44;	msg[18] = 0x45;
	msg[19] = 0x34;	msg[20] = 0x30;

	msg[21] = 0x0d;

	fprintf(stderr, "msg = %s\n", msg);
	write(fd, msg, 22);
	printf("send ok! \n");
}


/*函数描述:初始化串口设置*/
int InitSerialCom(int comNum,int baudRate,char parity,char nStop,char dataNum)
{
	int 	fd;
	struct  termios NewOpt,OldOpt;

	switch (comNum)
	{
		case 1:
			fd = open (TTYS_PORT0,O_RDWR) ;
			break;

		case 2:
			fd = open (TTYS_PORT1,O_RDWR | O_NOCTTY) ;
			break;

		case 3:
			fd = open (TTYS_PORT2,O_RDWR | O_NOCTTY) ;
			break;

		case 4:
			fd = open (TTYS_PORT3,O_RDWR | O_NOCTTY) ;
			break;

		default :
			fd = open (TTYS_PORT0,O_RDWR | O_NOCTTY) ;
			break;
	}
	if(fd < 0)
	{
		perror("can't not open the special com!");
		return -1;
	}
//	int ret;
//	ret = ioctl(fd, TCGETS, &OldOpt);
//	printf("ret = [%d]\n", ret);

	if(tcgetattr(fd, &OldOpt)!=0)      //获取打开的串口的属性,将属性保存到OldOpt中
	{
		perror("can not get serial attr!");
		close(fd);
		return -1;
	}
	NewOpt = OldOpt;

	SetSpeed(baudRate, &NewOpt);
	SetParity(parity, &NewOpt);
	SetStop( nStop,&NewOpt);
	SetDatanum(dataNum, &NewOpt);


	if(tcsetattr(fd, TCSANOW, &NewOpt) != 0 )
	{
		perror("tcsetattr error1");
		close (fd);
		return -1;
	}

	NewOpt.c_iflag   &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	NewOpt.c_oflag  &= ~OPOST;
	NewOpt.c_lflag   &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);

	NewOpt.c_cc[VMIN] 	= 3;
	NewOpt.c_cc[VTIME] 	= 1;

	tcflush(fd, TCIOFLUSH);
	if(tcsetattr(fd, TCSANOW, &NewOpt) != 0)
	{
		perror("tcsetattr error2");
		close (fd);
		return -1 ;
	}
	return fd;
}

int  serialReceive(int fd, char *RXBUF, int RX_len)
{

    int ret,n,pos,retval;
    fd_set rfds;
    struct timeval tv ;
    pos = 0;//指向接收缓冲
    FD_ZERO(&rfds);// 清空串口接收端口集
    FD_SET(fd,&rfds);// 设置串口接收端口集
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    retval = select(fd+1,&rfds,NULL,NULL,&tv);

    while(FD_ISSET(fd,&rfds)) // 检测串口是否有读写动作
    {
// 每次循环都要清空，否则不会检测到有变化
//		FD_ZERO(&rfds);// 清空串口接收端口集
//      FD_SET(fd,&rfds);// 设置串口接收端口集
//		retval = select(fd+1,&rfds,NULL,NULL,&tv);
        if(retval == -1)
        {
            perror("select()");
            break;
        }
        else if(retval)
        {   //判断是否还有数据
            //sleep(2);

            ioctl(fd,   FIONREAD,  &ret);
            if(ret <= 0)
            {
                break;
            }

            printf("%d bytes need to be readed\n", ret);
            ret = read(fd, RXBUF+pos, RX_len-pos);
            pos += ret;
			if(RXBUF[pos-1] == '#')
			{
				usleep(500*1000);
				break;
			}
            FD_ZERO(&rfds);// 清空串口接收端口集
            FD_SET(fd,&rfds);// 设置串口接收端口集
            retval = select(fd+1,&rfds,NULL,NULL,&tv);
 #if 0
            if((RXBUF[pos-2] == '\r') & (RXBUF[pos-1] == '\n')) // 确实接收到了数据，并打印出来
            {
                FD_ZERO(&rfds);
                FD_SET(fd,&rfds);
                retval = select(fd+1,&rfds,NULL,NULL,&tv);
                if(!retval)//no datas
                    break;
            }
#endif
        }
        else
            break;
    }

    return pos;
}

void  SerialSend(int fd, unsigned char *msg, int len)
{
	write(fd, msg, len);
}

#ifdef __cplusplus
}
#endif

