#ifndef _NETWORK_PUBLIC_H_
#define _NETWORL_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TCP_LISTEN_CON 5

typedef struct {

    short remote_port;
    short near_port;
    char remote_ip[32];
    char near_ip[32];

    int socket_fp;
    int multicast;
    //struct sockaddr_in socketaddr;

}udp_item_def;

//在监听之后，成功建立连接的描述
typedef struct {

    int bConnect;   //当前连接是否已经连接成功
    int conSocket;  //
    int IPValue;    //客户端建立连接的ip
    short remote_port; //客户端的port
    short reserve;
    int bSend;      //当前是否发送数据
    int bRecv;      //当前是否接受数据

}tcp_con_def;

//server用于监听的连接
typedef struct {

    short near_port;
    short reserve;
    char near_ip[32];

    int listen_sock;   //listion socket
    int connect_num; //connection number

    tcp_con_def con[MAX_TCP_LISTEN_CON]; //connection def

}tcp_server_item_def;

//client设置句柄
typedef struct {

    short near_port;
    short remote_port;
    char near_ip[32];
    char remote_ip[32];

    int socket_fp;
    int bRecv;
    int bSend;

}tcp_client_item_def;


int init_udp_instance(udp_item_def *pHandle);
int close_udp_instance(udp_item_def *pHandle);

int init_send_udp_instance(udp_item_def *pHandle);
int close_send_udp_instance(udp_item_def *pHandle);
int send_data_by_udp_instance(udp_item_def *pHandle, char *pData, int length);

int init_recv_udp_instance(udp_item_def *pHandle);
int close_recv_udp_instance(udp_item_def *pHandle);
int recv_data_by_udp_instance(udp_item_def *pHandle, char *pData, int *plength);

int init_tcp_server_instance(tcp_server_item_def *pHandle);
int close_tcp_server_instance(tcp_server_item_def *pHandle);
int tcp_server_recv_data(tcp_con_def *pHandle, char *pData, int *pLength);
int tcp_server_send_data(tcp_con_def *pHandle, char *pData, int *pLength);
tcp_con_def * tcp_find_unused_con(tcp_server_item_def *pHandle);
int tcp_safe_close_con(int sock_fp);
int tcp_server_accept(tcp_server_item_def *pHandle, tcp_con_def **ppCon);
int tcp_server_accept_1(tcp_server_item_def *pHandle, tcp_con_def **ppCon);
int tcp_server_accept_2(tcp_server_item_def *pHandle, tcp_con_def **ppCon);
int tcp_server_accept_3(tcp_server_item_def *pHandle, tcp_con_def **ppCon);

int tcp_server_close_con(tcp_con_def *pCon);

int init_tcp_client_instance(tcp_client_item_def *pHandle);
int tcp_client_connect(tcp_client_item_def *pHandle);
int tcp_client_send_data(tcp_client_item_def *pHandle, char *pData, int *pLength);
int tcp_client_recv_data(tcp_client_item_def *pHandle, char *pData, int *pLength);
int tcp_client_safe_close(tcp_client_item_def *pHandle);

int get_active_network_ip_and_mac(char *ip, unsigned char *mac, char *cardname);

#ifdef __cplusplus
}
#endif

#endif

