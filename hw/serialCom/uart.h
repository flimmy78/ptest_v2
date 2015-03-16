#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

int InitSerialCom(int comNum,int baudRate,char parity,char nStop,char dataNum);
void  SerialSend(int fd, unsigned char *msg, int len);

#ifdef __cplusplus
}
#endif

#endif

