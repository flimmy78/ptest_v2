
#include "sys_include.h"
//#include "debugPublic.h"
#include "networkPublic.h"

int init_tcp_server_instance(tcp_server_item_def *pHandle)
{
    struct sockaddr_in socketaddr;

    //open a TCP socket for server application to listen
    pHandle->listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (pHandle->listen_sock == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open the socket\r\n");
        return 0;
    }

    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(pHandle->near_port);
    socketaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(socketaddr.sin_zero), 0, 8);
    if (bind(pHandle->listen_sock,
        (struct sockaddr *) &(socketaddr),
        sizeof(struct sockaddr)) == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not bind the socket\r\n");
        return 0;
    }

    if (pHandle->connect_num >= MAX_TCP_LISTEN_CON)
    {
        pHandle->connect_num = MAX_TCP_LISTEN_CON;
        M_TRACE(WARING_TRACE, NET_MODE, "connection is too big and set default\r\n");
    }

    {
        int i;
        tcp_con_def *pCon;

        for (i = 0; i < pHandle->connect_num; i++)
        {
            pCon = &(pHandle->con[i]);
            pCon->bConnect = 0;
            pCon->conSocket = -1;
            pCon->IPValue = 0;
            pCon->remote_port = 0;
            pCon->reserve = 0;
            pCon->bRecv = 0;
            pCon->bSend = 0;
        }
    }

    if(listen(pHandle->listen_sock, pHandle->connect_num) < 0)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "listen error\r\n");
        return 0;
    }

    M_TRACE(DEBUG_TRACE, NET_MODE, "init recv tcp instance success\r\n");

    return 1;

}

int close_tcp_server_instance(tcp_server_item_def *pHandle)
{
    int i;
    for (i = 0; i < pHandle->connect_num; i++)
    {
        tcp_con_def * pCon;
        pCon = &(pHandle->con[i]);
        tcp_server_close_con(pCon);
    }

    tcp_safe_close_con(pHandle->listen_sock);

    return 1;
}

int tcp_server_recv_data(tcp_con_def *pHandle, char *pData, int *pLength)
{
    int length;

    length = *pLength;
    pHandle->bRecv = 1;
    length = recv(pHandle->conSocket, pData, length, 0);
    pHandle->bRecv = 0;
    *pLength = length;

    return length;
}

int tcp_server_send_data(tcp_con_def *pHandle, char *pData, int *pLength)
{
    int length;

    length = *pLength;
    pHandle->bSend = 1;
    length = send(pHandle->conSocket, pData, length, 0);
    pHandle->bSend = 0;
    *pLength = length;

    return length;
}

//找到一个可用的连接，用于accept
tcp_con_def * tcp_find_unused_con(tcp_server_item_def *pHandle)
{
    int i;
    tcp_con_def *pCon;

    for (i = 0; i < pHandle->connect_num; i ++)
    {
        pCon = &(pHandle->con[i]);
        if (pCon->bConnect == 0)
        {
            return pCon;
        }

    }

    return NULL;
}

int tcp_safe_close_con(int sock_fp)
{
    int ret;

    ret = shutdown(sock_fp, 2);
    if (ret == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "failure\r\n");
    }

    close(sock_fp);

    return 1;
}

int tcp_server_close_con(tcp_con_def *pCon)
{
    if (pCon == NULL)
    {
        return 0;
    }

    if (pCon->bConnect)
    {
        tcp_safe_close_con(pCon->conSocket);
        pCon->bConnect = 0;
        pCon->bRecv = 0;
        pCon->bSend = 0;
        return 1;
    }
    return 0;
}
int tcp_server_accept(tcp_server_item_def *pHandle, tcp_con_def **ppCon)
{
    tcp_con_def *pCon;
    unsigned int size_len;
    struct sockaddr_in client_addr;

    pCon = tcp_find_unused_con(pHandle);
    if (pCon == NULL)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "can not find a useable ");
        return 0;
    }

    size_len = sizeof(struct sockaddr);

    pCon->conSocket = accept(pHandle->listen_sock,
        (struct sockaddr *) &client_addr,
        &size_len);
    if (pCon->conSocket == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "accept failure\r\n");
        return 0;
    }
    *ppCon = pCon;
    pCon->bConnect = 1;
    pCon->remote_port = ntohs(client_addr.sin_port);
    pCon->IPValue = ntohl(client_addr.sin_addr.s_addr);
    pCon->bRecv = 0;
    pCon->bSend = 0;

    M_TRACE(DEBUG_TRACE, NET_MODE, "accept success port = %d, IP0x%x\r\n", pCon->remote_port, pCon->IPValue);

    return 1;
}

int tcp_server_accept_1(tcp_server_item_def *pHandle, tcp_con_def **ppCon)
{
    tcp_con_def *pCon;
    unsigned int size_len;
    struct sockaddr_in client_addr;
/*
    pCon = tcp_find_unused_con(pHandle);
    if (pCon == NULL)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "can not find a useable ");
        return 0;
    }
*/
    pCon = &(pHandle->con[0]);
    size_len = sizeof(struct sockaddr);

    pCon->conSocket = accept(pHandle->listen_sock,
        (struct sockaddr *) &client_addr,
        &size_len);
    if (pCon->conSocket == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "accept failure\r\n");
        return 0;
    }
    *ppCon = pCon;
    pCon->bConnect = 1;
    pCon->remote_port = ntohs(client_addr.sin_port);
    pCon->IPValue = ntohl(client_addr.sin_addr.s_addr);
    pCon->bRecv = 0;
    pCon->bSend = 0;

    M_TRACE(DEBUG_TRACE, NET_MODE, "accept success port = %d, IP0x%x\r\n", pCon->remote_port, pCon->IPValue);

    return 1;
}

int tcp_server_accept_2(tcp_server_item_def *pHandle, tcp_con_def **ppCon)
{
    tcp_con_def *pCon;
    unsigned int size_len;
    struct sockaddr_in client_addr;
/*
    pCon = tcp_find_unused_con(pHandle);
    if (pCon == NULL)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "can not find a useable ");
        return 0;
    }
*/
    pCon = &(pHandle->con[1]);
    size_len = sizeof(struct sockaddr);

    pCon->conSocket = accept(pHandle->listen_sock,
        (struct sockaddr *) &client_addr,
        &size_len);
    if (pCon->conSocket == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "accept failure\r\n");
        return 0;
    }
    *ppCon = pCon;
    pCon->bConnect = 1;
    pCon->remote_port = ntohs(client_addr.sin_port);
    pCon->IPValue = ntohl(client_addr.sin_addr.s_addr);
    pCon->bRecv = 0;
    pCon->bSend = 0;

    M_TRACE(DEBUG_TRACE, NET_MODE, "accept success port = %d, IP0x%x\r\n", pCon->remote_port, pCon->IPValue);

    return 1;
}

int tcp_server_accept_3(tcp_server_item_def *pHandle, tcp_con_def **ppCon)
{
    tcp_con_def *pCon;
    unsigned int size_len;
    struct sockaddr_in client_addr;
/*
    pCon = tcp_find_unused_con(pHandle);
    if (pCon == NULL)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "can not find a useable ");
        return 0;
    }
*/
    pCon = &(pHandle->con[2]);

    size_len = sizeof(struct sockaddr);

    pCon->conSocket = accept(pHandle->listen_sock,
        (struct sockaddr *) &client_addr,
        &size_len);
    if (pCon->conSocket == -1)
    {
        M_TRACE(WARING_TRACE, NET_MODE, "accept failure\r\n");
        return 0;
    }
    *ppCon = pCon;
    pCon->bConnect = 1;
    pCon->remote_port = ntohs(client_addr.sin_port);
    pCon->IPValue = ntohl(client_addr.sin_addr.s_addr);
    pCon->bRecv = 0;
    pCon->bSend = 0;

    M_TRACE(DEBUG_TRACE, NET_MODE, "accept success port = %d, IP0x%x\r\n", pCon->remote_port, pCon->IPValue);

    return 1;
}

