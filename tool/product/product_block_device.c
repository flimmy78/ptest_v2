#include "sys_include.h"

#include "product_public.h"

int confirm_tfcard_running(void)
{
    //system("mount -t auto -o iocharset=utf8 /dev/mmcblk1p1 /media/tfcard");

    if (access("/mnt/sd/160001.MPG", F_OK) == 0)
    {
        printf("\nsdcard success\n");
        set_test_status(sdcard_ok);
        return 1;
    }

    system("mount -t auto /dev/mmcblk1p1 /mnt/sd");

    if (access("/mnt/sd/160001.MPG", F_OK) == 0)
    {
        printf("\nsdcard success\n");
        set_test_status(sdcard_ok);
        return 1;
    }

    printf("\nsdcard success\n");

    return 0;
}


static int get_file_md5_code(char *filename, char *md5)
{
    FILE *rfp = NULL;

    char cmd[256];
    memset(cmd, 0, 256);
    sprintf(cmd, "md5sum %s > /tmp/md5code", filename);
    system(cmd);

    rfp = fopen("/tmp/md5code", "rb");
    if (rfp)
    {
        fread(md5, 1, 32, rfp);
        fclose(rfp);
        M_TRACE(DEBUG_TRACE, FILE_MODE, "%s\n", md5);
        return 1;
    }

    return 0;
}

int confirm_usb_running(void)
{
    int i;
    char md5code[128];
    system("mkdir -p /mnt/usb");

    for (i = 0; i < 5; i ++)
    {
        system("mount -t vfat /dev/sda /mnt/usb | mount -t vfat /dev/sdb /mnt/usb | mount -t auto /dev/sdb1 /mnt/usb");
        usleep(500*1000);

        if (access("/mnt/usb/md5code", F_OK) == 0)
        {
            memset(md5code, 0, 128);

            system("cp /mnt/usb/md5code /mnt/usb/md5code1");
            get_file_md5_code("/mnt/usb/md5code1", md5code);

            if (strncmp("5b5ac771b8fba86e2a9b50c02cefc668", md5code, strlen(md5code)) == 0)
            {
                printf("\nusb success\n");
            }
            set_test_status(usb_ok);
            return 1;
        }

        usleep(1000*1000);
    }

    printf("\nusb failure\n");
    return 0;
}

int confirm_msata_running(void)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        system("mount -t ext4 /dev/sda1 /mnt/sda | mount -t ext4 /dev/sda /mnt/sda");

        usleep(500*1000);

        if (access("/mnt/sda/website/index.html", F_OK) == 0)
        {
            printf("\nmsata success\n");
            set_test_status(msata_ok);
            return 1;
        }

        usleep(1000*1000);
    }

    printf("\nmsata failure\n");
    return 0;
}

