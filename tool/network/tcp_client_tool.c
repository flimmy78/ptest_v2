#include "sys_include.h"
//#include "debugPublic.h"
#include "networkPublic.h"

int init_tcp_client_instance(tcp_client_item_def *pHandle)
{
    struct sockaddr_in socketaddr;

    //open a TCP socket for client application to connect
    pHandle->socket_fp = socket(AF_INET, SOCK_STREAM, 0);
    if (pHandle->socket_fp == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open the socket\r\n");
        return 0;
    }

    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(pHandle->near_port);
    socketaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(socketaddr.sin_zero), 0, 8);
    if (bind(pHandle->socket_fp,
        (struct sockaddr *) &(socketaddr),
        sizeof(struct sockaddr)) == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not bind the socket\r\n");
        return 0;
    }

    pHandle->bRecv = 0;
    pHandle->bSend = 0;
    M_TRACE(DEBUG_TRACE, NET_MODE, "init tcp client instance success\r\n");

    return 1;
}

int tcp_client_connect(tcp_client_item_def *pHandle)
{
    int ret;
    struct sockaddr_in socketaddr;

    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(pHandle->remote_port);
    socketaddr.sin_addr.s_addr = inet_addr(pHandle->remote_ip);
    memset(&(socketaddr.sin_zero), 0, 8);


    ret = connect(pHandle->socket_fp,
                (struct sockaddr *)&socketaddr,
                 sizeof(struct sockaddr));
    if (ret == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "connect failure\r\n");
        return 0;
    }

    return 1;
}

int tcp_client_send_data(tcp_client_item_def *pHandle, char *pData, int *pLength)
{
    int length;

    length = *pLength;
    pHandle->bSend = 1;
    length = send(pHandle->socket_fp, pData, length, 0);
    *pLength = length;
    pHandle->bSend = 0;

    return length;
}

int tcp_client_recv_data(tcp_client_item_def *pHandle, char *pData, int *pLength)
{
    int length;

    length = *pLength;
    pHandle->bRecv = 1;
    length = recv(pHandle->socket_fp, pData, length, 0);
    *pLength = length;
    pHandle->bRecv = 0;
    return length;
}

int tcp_client_safe_close(tcp_client_item_def *pHandle)
{
    int ret;
    ret = shutdown(pHandle->socket_fp, 2);
    if (ret == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "shutdown failure\r\n");
    }

    close(pHandle->socket_fp);

    return 1;
}

