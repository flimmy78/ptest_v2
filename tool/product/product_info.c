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

    system("atenv infos/product/mac > /tmp/mac.ini");

    rfp = fopen("/tmp/mac.ini", "rb");
    if (rfp == NULL)
    {
        printf("\nmac read failure\n");
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

    system("atenv infos/product/sn > /tmp/sn.ini");

    rfp = fopen("/tmp/sn.ini", "rb");
    if (rfp == NULL)
    {
        printf("\nsn read failure\n");
        return 0;
    }

    length = fread(sn, 1, 26, rfp);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "sn=%s,%d\n", sn, length);
    fclose(rfp);

    return length;
}

int get_pcba_iccid(char *iccid, int length)
{
    FILE *rfp = NULL;

    //system("bootm pcba.iccid > /tmp/iccid.ini");

    rfp = fopen("/tmp/.3g/iccid", "rb");
    if (rfp == NULL)
    {
        printf("\niccid read failure\n");
        return 0;
    }

    length = fread(iccid, 1, 26, rfp);
    M_TRACE(DEBUG_TRACE, SYS_MODE, "iccid=%s,%d\n", iccid, length);
    fclose(rfp);

    return length;
}


