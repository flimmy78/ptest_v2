/***************************************************************************
  文件:IPCComunicationABS.c
  作者:Macros.Zhang
  日期:2007-11-20
  版本:1.0


  描述:进程间消息通信功能抽象

  其他:
****************************************************************************/
#include "sys_include.h"
#include "my_msg.h"

static int msgqueueArray[REGISTER_MSG_ID_MAX];
static key_t baseKey = 0x8880;

static int bInit = 0;

/*
    初始化消息队列控制函数
*/
int initMsgQueue(void)
{
    int i;

    if (bInit)
    {
        return 0;
    }

    bInit = 1;
    for (i = 0; i < REGISTER_MSG_ID_MAX; i++)
    {
        msgqueueArray[i] = -1;
    }

    return 0;
}

/*
    根据消息队列ID,创建消息队列.
*/
int createMsgQueue(int registerMsgID)
{
    key_t key;
    int msgqueue_id;
    ipcMsgQueueData_t msg;
    int ret;

    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    initMsgQueue();

    key = baseKey+registerMsgID;
    if (msgqueueArray[registerMsgID] == -1)
    {
        if ((msgqueue_id = msgget(key,IPC_CREAT | 0660 )) == -1 )
        {
            fprintf(stderr, "create msg queue is failed\r\n");
            return -1;
        }

        msgqueueArray[registerMsgID] = msgqueue_id;

        //清理msg队列里面多余的消息
        while(1)
        {
            ret = recvMsgQueueTimeOut(registerMsgID, &msg, 100);
            if (ret < 0)
            {
                break;
            }
        }
        return msgqueue_id;
    }

    return -3;

}

/*
    删除消息队列,如果设计多线程通讯，这个函数原则上不要用.
*/
int deleteMsgQueue(int registerMsgID)
{
    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    msgctl(msgqueueArray[registerMsgID], IPC_RMID, NULL);/*删除队列*/

    return 0;
}

/*
    清空消息队列
*/
int clearMsgQueue(int registerMsgID)
{
    struct msqid_ds bufQidDs;

    ipcMsgQueueData_t Msg;
    int ret;

    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    if( msgctl( msgqueueArray[registerMsgID], IPC_STAT, &bufQidDs) == -1)
    {
        return(-1);
    }

    //printf("msgNum = %d\n", bufQidDs.msg_qnum);
    if (bufQidDs.msg_qnum > 0)
    {
        while(bufQidDs.msg_qnum)
        {

            ret = msgrcv(msgqueueArray[registerMsgID],
                                &Msg,
                                IPC_MSG_DATA_MAX_LENGTH,
                                0,
                                IPC_NOWAIT);

            if (ret == -1)
            {
                break;
            }
        }
    }

    return 0;
}

/*
    发送消息按照消息队列id
*/
int sendMsgQueue(int registerMsgID, ipcMsgQueueData_t *pMsg)
{
    int ret;
    struct msqid_ds msg_info;

    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    if (pMsg == NULL)
    {
        return -4;
    }

    msgctl(msgqueueArray[registerMsgID], IPC_STAT, &msg_info);
    if (msg_info.msg_qnum > 0)
    {
        //fprintf(stderr, "drop the msg %ld, %x\n", msg_info.msg_qnum, pMsg->msg_type);
        ret = -5;
    }
    else
    {
        //fprintf(stderr, "msg->type = %x\n", pMsg->msg_type);
        ret = msgsnd(msgqueueArray[registerMsgID], pMsg, IPC_MSG_DATA_MAX_LENGTH-4, 0);
    }

    return ret;
}

int sendMsgQueueWithoutDrop(int registerMsgID, ipcMsgQueueData_t *pMsg)
{
    int ret;

    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    if (pMsg == NULL)
    {
        return -4;
    }

    ret = msgsnd(msgqueueArray[registerMsgID], pMsg, IPC_MSG_DATA_MAX_LENGTH-4, 0);

    return ret;
}


/*
    无条件等待，接收消息
*/
int recvMsgQueueWaitForever(int registerMsgID, ipcMsgQueueData_t *pMsg)
{
    int ret;
    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    if (pMsg == NULL)
    {
        return -4;
    }

    ret = msgrcv(msgqueueArray[registerMsgID],
                    pMsg,
                    IPC_MSG_DATA_MAX_LENGTH,
                    0,
                    0);

    return ret;
}

/*
    设定有超时等待接收消息
    int msTimeout; 50ms精度的等待超时时间设定;
    如果需要等待50ms，则设定50
*/
int recvMsgQueueTimeOut(int registerMsgID, ipcMsgQueueData_t *pMsg, int msTimeout)
{
    int ret = 0;
    int Time = 0;
    if (registerMsgID >= REGISTER_MSG_ID_MAX)
    {
        return -2;
    }

    if (msgqueueArray[registerMsgID] == -1)
    {
        return -3;
    }

    if (pMsg == NULL)
    {
        return -4;
    }

    do
    {
        ret = msgrcv(msgqueueArray[registerMsgID],
                        pMsg,
                        IPC_MSG_DATA_MAX_LENGTH,
                        0,
                        IPC_NOWAIT);
        if (ret < 0)
        {
//            printf("no msg");
        }
        else
        {
            break;
        }


        if (Time >= msTimeout)
        {
            ret = -1;
            break;
        }

        Time += IPC_CHECK_MSG_QUEUE_INTERVAL;
        usleep(IPC_CHECK_MSG_QUEUE_INTERVAL*1000);
    }while(1);

    return ret;
}


