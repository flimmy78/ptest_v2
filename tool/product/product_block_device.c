#include "sys_include.h"

#include "product_public.h"

int confirm_tfcard_running(void)
{
    //system("mount -t auto -o iocharset=utf8 /dev/mmcblk1p1 /media/tfcard");

    if (access("/mnt/sd/160001.MPG", F_OK) == 0)
    {
        return 1;
    }

    return 0;
}

int confirm_msata_running(void)
{
    system("mount -t ext4 /dev/sda /mnt/sda");
    if (access("/mnt/sda/lost+found", F_OK) == 0)
    {
        return 1;
    }

    return 0;
}

