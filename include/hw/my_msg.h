#ifndef _MY_MSG_H_
#define _MY_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 已经注册的可以使用的进程间通信队列ID */
enum
{
    RTC_ID,
    _RTC_ID,

    TMP_ID,
    _TMP_ID,

    AI_ID,
    _AI_ID,

    AV_ENGINE_ID,
    _AV_ENGINE_ID,

	REGISTER_MSG_ID_MAX
};

#define IPC_MSG_DATA_MIN_LENGTH 32
#define IPC_MSG_DATA_MAX_LENGTH 512
#define IPC_CHECK_MSG_QUEUE_INTERVAL 50

typedef struct IPC_MSG_QUEUE_DATA
{
	int msg_type;
	char msg_data[IPC_MSG_DATA_MAX_LENGTH];
}ipcMsgQueueData_t;

/*
    初始化消息队列控制函数
*/
int initMsgQueue(void);

/*
    根据消息队列ID,创建消息队列.
*/
int createMsgQueue(int registerMsgID);

/*
    删除消息队列,如果设计多线程通讯，这个函数原则上不要用.
*/
int deleteMsgQueue(int registerMsgID);

/*
    清空消息队列
*/
int clearMsgQueue(int registerMsgID);

/*
    发送消息按照消息队列id
*/
int sendMsgQueue(int registerMsgID, ipcMsgQueueData_t *pMsg);

int sendMsgQueueWithoutDrop(int registerMsgID, ipcMsgQueueData_t *pMsg);

/*
    无条件等待，接收消息
*/
int recvMsgQueueWaitForever(int registerMsgID, ipcMsgQueueData_t *pMsg);

/*
    设定有超时等待接收消息
    int msTimeout; 100ms精度的等待超时时间设定;
    如果需要等待100ms
*/
int recvMsgQueueTimeOut(int registerMsgID, ipcMsgQueueData_t *pMsg, int msTimeout);

#ifdef __cplusplus
}
#endif

#endif


