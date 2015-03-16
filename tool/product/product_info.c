#include "sys_include.h"
#include "product_public.h"

int set_pcba_mac(char *mac, int length)
{
    char cmd[256];

    memset(cmd, 0, 256);

    sprintf(cmd, "bootm pcba.mac=%s", mac);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "%s\n", cmd);

    system(cmd);
    return 1;
}

int get_pcba_mac(char *mac, int length)
{
    FILE *rfp = NULL;

    system("bootm pcba.mac > /tmp/mac.ini");

    rfp = fopen("/tmp/mac.ini", "rb");
    if (rfp == NULL)
    {
        return 0;
    }

    length = fread(mac, 1, 20, rfp);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "mac=%s,%d\n", mac, length);
    fclose(rfp);

    return length;
}

int set_pcba_sn(char *sn, int length)
{
    char cmd[256];

    memset(cmd, 0, 256);

    sprintf(cmd, "bootm pcba.sn=%s", sn);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "%s\n", cmd);

    system(cmd);
    return 1;
}

int get_pcba_sn(char *sn, int length)
{
    FILE *rfp = NULL;

    system("bootm pcba.sn > /tmp/sn.ini");

    rfp = fopen("/tmp/sn.ini", "rb");
    if (rfp == NULL)
    {
        return 0;
    }

    length = fread(sn, 1, 26, rfp);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "sn=%s,%d\n", sn, length);
    fclose(rfp);

    return length;
}


