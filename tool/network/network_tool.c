#include "sys_include.h"
#include "networkPublic.h"

#define MAXINTERFACES 8

int get_active_network_ip_and_mac(char *ip, unsigned char *mac, char *cardname)
{
    int socket_fp;
    int ret;
    int intrface;
    //int retn;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;
    //struct arpreq arp;

    socket_fp = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fp == -1)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "can not open socket\r\n");
        return 0;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    ret = ioctl(socket_fp, SIOCGIFCONF, (char *) &ifc);
    if (ret < 0)
    {
        M_TRACE(ERROR_TRACE, NET_MODE, "SIOCGIFCONF failure\r\n");
        close(socket_fp);
        return 0;
    }
    intrface = ifc.ifc_len/sizeof(struct ifreq);
    M_TRACE(DEBUG_TRACE, NET_MODE, "interface num is intrface = %d\r\n", intrface);

    while(intrface-- > 0)
    {
        if (strncmp(buf[intrface].ifr_name, "lo", strlen("lo")) == 0)
        {
            continue;
        }

        M_TRACE(DEBUG_TRACE, NET_MODE, "net device %s\r\n", buf[intrface].ifr_name);
        ret = ioctl(socket_fp, SIOCGIFFLAGS, (char *)&buf[intrface]);
        if (ret < 0)
        {
            M_TRACE(DEBUG_TRACE, NET_MODE, "SIOCGIFFLAGS failure\r\n");
            continue;
        }

        M_TRACE(DEBUG_TRACE, NET_MODE, "card running flags%x\r\n", buf[intrface].ifr_flags);

        if (strncmp(buf[intrface].ifr_name, "eth0", strlen("eth0")) == 0)
        {
            if ((buf[intrface].ifr_flags & IFF_UP) &&
                (buf[intrface].ifr_flags & IFF_RUNNING))
            {
                ret = ioctl(socket_fp, SIOCGIFADDR, (char *) &buf[intrface]);
                if (!ret)
                {
                    char *p = NULL;

                    p = inet_ntoa(((struct sockaddr_in *)(&buf[intrface].ifr_addr))->sin_addr);
                    M_TRACE(DEBUG_TRACE, NET_MODE, "%s|%d\r\n", p, strlen(p));
                    if (ip)
                    {
                        memcpy(ip, p, strlen(p));
                    }
                }

                ret = ioctl(socket_fp, SIOCGIFHWADDR, (char *)(&buf[intrface]));
                if (!ret)
                {
                    unsigned char *m = NULL;
                    m = (unsigned char *) &(buf[intrface].ifr_hwaddr.sa_data);
                    M_TRACE(DEBUG_TRACE, NET_MODE, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                        m[0], m[1], m[2], m[3], m[4], m[5]);
                    if (mac)
                    {
                        memcpy(mac, m, 6);
                    }

                    if (cardname)
                    {
                        memcpy(cardname, "eth0", strlen("eth0"));
                    }
                }
            }
        }

        if (strncmp(buf[intrface].ifr_name, "ra0", strlen("ra0")) == 0)
        {
            if ((buf[intrface].ifr_flags & IFF_UP) &&
                (buf[intrface].ifr_flags & IFF_BROADCAST))
            {
                ret = ioctl(socket_fp, SIOCGIFADDR, (char *) &buf[intrface]);
                if (!ret)
                {
                    char *p = NULL;

                    p = inet_ntoa(((struct sockaddr_in *)(&buf[intrface].ifr_addr))->sin_addr);
                    M_TRACE(DEBUG_TRACE, NET_MODE, "%s|%d\r\n", p, strlen(p));
                    if (ip)
                    {
                        memcpy(ip, p, strlen(p));
                    }
                }

                ret = ioctl(socket_fp, SIOCGIFHWADDR, (char *)(&buf[intrface]));
                if (!ret)
                {
                    unsigned char *m = NULL;
                    m = (unsigned char *) &(buf[intrface].ifr_hwaddr.sa_data);
                    M_TRACE(DEBUG_TRACE, NET_MODE, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                        m[0], m[1], m[2], m[3], m[4], m[5]);
                    if (mac)
                    {
                        memcpy(mac, m, 6);
                    }

                    if (cardname)
                    {
                        memcpy(cardname, "ra0", strlen("ra0"));
                    }
                }
            }

        }
    }

    close(socket_fp);
    return 1;
}

