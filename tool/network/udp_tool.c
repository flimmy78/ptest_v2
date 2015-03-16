#include "sys_include.h"
//#include "debugPublic.h"
#include "networkPublic.h"

int init_udp_instance(udp_item_def *pHandle)
{
    struct sockaddr_in socketaddr;
    struct timeval timeout;

    //open a udp socket
    pHandle->socket_fp = socket(AF_INET, SOCK_DGRAM, 0);
    if (pHandle->socket_fp == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open the socket\r\n");
        return 0;
    }

    //set the timeout 1ms

    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    setsockopt(pHandle->socket_fp,
        SOL_SOCKET, SO_SNDTIMEO,
        (const char*)&timeout,
        sizeof(timeout));

    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    setsockopt(pHandle->socket_fp,
        SOL_SOCKET, SO_RCVTIMEO,
        (const char*)&timeout,
        sizeof(timeout));

    //bind the socket and set the local port and ip address;
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


    if (pHandle->multicast)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(pHandle->remote_ip);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if(setsockopt(pHandle->socket_fp, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0)
        {
            M_TRACE(ERROR_TRACE, NET_MODE, "can not add the multicast failed!\n");
            return -1;
        }
    }

    M_TRACE(DEBUG_TRACE, NET_MODE, "init udp instance success\r\n");
    return 1;
}

int close_udp_instance(udp_item_def *pHandle)
{
    if (pHandle->socket_fp != -1)
    {
        close(pHandle->socket_fp);
        M_TRACE(DEBUG_TRACE, NET_MODE, "close udp socket\r\n");
        return 1;
    }

    M_TRACE(ERROR_TRACE, NET_MODE, "close udp socket failure\r\n");
    return 0;
}


int init_send_udp_instance(udp_item_def *pHandle)
{
    struct sockaddr_in socketaddr;
    struct timeval timeout;

    //open a udp socket
    pHandle->socket_fp = socket(AF_INET, SOCK_DGRAM, 0);
    if (pHandle->socket_fp == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open the socket\r\n");
        return 0;
    }

    //set the timeout 1ms
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    setsockopt(pHandle->socket_fp,
        SOL_SOCKET, SO_SNDTIMEO,
        (const char*)&timeout,
        sizeof(timeout));

    //bind the socket and set the local port and ip address;
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

    M_TRACE(DEBUG_TRACE, NET_MODE, "init send udp instance success\r\n");
    return 1;
}

int close_send_udp_instance(udp_item_def *pHandle)
{
    if (pHandle->socket_fp != -1)
    {
        close(pHandle->socket_fp);
        M_TRACE(DEBUG_TRACE, NET_MODE, "close send udp socket\r\n");
        return 1;
    }

    M_TRACE(ERROR_TRACE, NET_MODE, "close send udp socket failure\r\n");
    return 0;
}

int send_data_by_udp_instance(udp_item_def *pHandle, char *pData, int length)
{
    struct sockaddr_in socketaddr;
    int sendlen = 0;

    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(pHandle->remote_port);
    socketaddr.sin_addr.s_addr = inet_addr(pHandle->remote_ip);

    sendlen = sendto(pHandle->socket_fp,
                pData, length,
                0,
                (struct sockaddr *) &(socketaddr),
                sizeof(struct sockaddr));

    return sendlen;
}

int init_recv_udp_instance(udp_item_def *pHandle)
{
    struct sockaddr_in socketaddr;
    struct timeval timeout;

    //open a udp socket
    pHandle->socket_fp = socket(AF_INET, SOCK_DGRAM, 0);
    if (pHandle->socket_fp == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open the socket\r\n");
        return 0;
    }

    //set the timeout 1ms
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    setsockopt(pHandle->socket_fp,
        SOL_SOCKET, SO_RCVTIMEO,
        (const char*)&timeout,
        sizeof(timeout));

    //bind the socket and set the local port and ip address;
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

    M_TRACE(DEBUG_TRACE, NET_MODE, "init recv udp instance success\r\n");
    return 1;
}

int close_recv_udp_instance(udp_item_def *pHandle)
{
    if (pHandle->socket_fp != -1)
    {
        close(pHandle->socket_fp);
        M_TRACE(DEBUG_TRACE, NET_MODE, "close recv udp socket\r\n");
        return 1;
    }

    M_TRACE(ERROR_TRACE, NET_MODE, "close recv udp socket failure\r\n");
    return 0;
}

int recv_data_by_udp_instance(udp_item_def *pHandle, char *pData, int *plength)
{
    struct sockaddr_in socketaddr;
    int recvlen;
    int sockaddlen;

    sockaddlen = sizeof(struct sockaddr_in);
    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(pHandle->remote_port);
    if (pHandle->remote_ip[0] == 0)
    {
        socketaddr.sin_addr.s_addr = INADDR_ANY; //inet_addr(pHandle->remote_ip);//INADDR_ANY;
    }
    else
    {
        socketaddr.sin_addr.s_addr = inet_addr(pHandle->remote_ip);
    }
    //memset(&(socketaddr.sin_zero), 0, 8);

    recvlen = *plength;
    recvlen = recvfrom(pHandle->socket_fp,
                pData, 1500,
                0,
                (struct sockaddr *)&socketaddr,
                (unsigned int *)&sockaddlen);
    //fprintf(stderr, "recvlen %d\n", recvlen);
    return recvlen;
}


