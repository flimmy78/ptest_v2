#include "sys_include.h"
#include "product_public.h"
#include "networkPublic.h"

#define test_ip_addr "192.168.1."

#define server_ip_addr "192.168.1.107"
#define port_base 10000

int set_test_network(int number)
{
    char ip_address[32];
    char cmd[256];

    memset(cmd, 0, 256);

    memset(ip_address, 0, 32);
    sprintf(ip_address, "%s", test_ip_addr);

    if (number > 0 && number < 90)
    {
        sprintf(ip_address, "%s%d", ip_address, (number+2));

        sprintf(cmd, "ifconfig eth0 %s netmask 255.255.255.0", ip_address);

        system(cmd);

        M_TRACE(DEBUG_TRACE, SYS_MODE, "%s\n", cmd);
        return 1;
    }

    return 0;
}

int confirm_network_running(int num)
{
    int port;
    udp_item_def Handle;
    char data[100];
    int length;
    int ret = 0;
    int i;

    port = port_base+num+2;
    Handle.near_port = 1000;
    Handle.remote_port = port;
    Handle.multicast = 1;
    memset(Handle.near_ip, 0, 32);

    memset(Handle.remote_ip, 0, 32);
    memcpy(Handle.remote_ip, server_ip_addr, strlen(server_ip_addr));

    Handle.socket_fp = -1;

    init_send_udp_instance(&Handle);

    memset(data, 0, 100);
    sprintf(data, "%s", test_ip_addr);
    sprintf(data, "%s%d", data, (num+2));
    length = strlen(data);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "%s length = %d\n", data, length);
    ret = send_data_by_udp_instance(&Handle, data, length);
    if (ret != length)
    {
        M_TRACE(DEBUG_TRACE, SYS_MODE, "error\n");
        return 0;
    }

    ret = send_data_by_udp_instance(&Handle, data, length);
    if (ret != length)
    {

        M_TRACE(DEBUG_TRACE, SYS_MODE, "error\n");
        return 0;
    }

    ret = send_data_by_udp_instance(&Handle, data, length);
    if (ret != length)
    {

        M_TRACE(DEBUG_TRACE, SYS_MODE, "error\n");
        return 0;
    }

    close_send_udp_instance(&Handle);

    Handle.near_port = 1002;
    Handle.remote_port = 1001;
    Handle.multicast = 1;
    memset(Handle.near_ip, 0, 32);
    memset(Handle.remote_ip, 0, 32);
    memcpy(Handle.remote_ip, server_ip_addr, strlen(server_ip_addr));
    init_recv_udp_instance(&Handle);

    memset(data, 0, 100);
    for (i = 0; i < 3; i ++)
    {
	    length = 100;
        memset(Handle.remote_ip, 0, 32);
	    ret = recv_data_by_udp_instance(&Handle, data, &length);
	    if (ret > 0)
	    {
	        M_TRACE(DEBUG_TRACE, SYS_MODE, "%d length = %d\n", data, length);
	        return 1;
	    }
  	}

    close_recv_udp_instance(&Handle);
    return 0;
}


