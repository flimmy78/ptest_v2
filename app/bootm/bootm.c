
#include "sys_include.h"
#include "crc32.h"

typedef struct {

    unsigned int crc_flag;
    unsigned char flag;
    unsigned char a_flags;
    unsigned char b_flags;
    unsigned char c_flags;
    unsigned char err_cnt;

    char *data;

}boot_argv_def;


int get_bootm_argc_length(char *data)
{
    int i;
    int length = 0;
    char *point;

    point = data;
    for (i = 0; i < 2048; i++)
    {
        if ((point[i] == 0 &&
            point[i+1] == 0))
        {
            break;
        }

        length ++;
    }

    return length;
}

static int config_flags(unsigned char *pflags, unsigned char *pdata)
{
    if (strncmp((char *)pdata, "flags=", strlen("flags=")) == 0)
    {
        *pflags = (pdata[strlen("flags=")] - 48);
        return 1;
    }

    return 0;
}

static int config_a_flags(unsigned char *pflags, unsigned char *pdata)
{
    if (strncmp((char *)pdata, "a_flags=", strlen("a_flags=")) == 0)
    {
        *pflags = (pdata[strlen("a_flags=")] - 48);
        return 1;
    }

    return 0;
}

static int config_b_flags(unsigned char *pflags, unsigned char *pdata)
{
    if (strncmp((char *)pdata, "b_flags=", strlen("b_flags=")) == 0)
    {
        *pflags = (pdata[strlen("b_flags=")] - 48);
        return 1;
    }

    return 0;
}

static int config_c_flags(unsigned char *pflags, unsigned char *pdata)
{
    if (strncmp((char *)pdata, "c_flags=", strlen("c_flags=")) == 0)
    {
        *pflags = (pdata[strlen("c_flags=")] - 48);
        return 1;
    }

    return 0;
}

static int config_err_cnt(unsigned char *pflags, unsigned char *pdata)
{
    if (strncmp((char *)pdata, "err_cnt=", strlen("err_cnt=")) == 0)
    {
        *pflags = (pdata[strlen("err_cnt=")] - 48);
        return 1;
    }

    return 0;
}



#if 0
int main( int argc, char **argv )
{
    FILE *rfp = NULL;
    int length;
    unsigned int crc_value;
    boot_argv_def *pbootargv = NULL;
    unsigned char *argvdata = NULL;

    int i, j;
    unsigned char flags = 255;
    unsigned char a_flags = 255;
    unsigned char b_flags = 255;
    unsigned char c_flags = 255;
    unsigned char err_cnt = 255;
    int ret = 0;

    int bsave = 0;

    for (i = 0; i < 5; i++)
    {
        for (j = 1; j < argc; j++)
        {
            if (i == 0)
            {
                ret = config_flags(&flags, (unsigned char *)argv[j]);
                if (ret)
                {
                    break;
                }

            }
            else if (i == 1)
            {
                ret = config_a_flags(&a_flags, (unsigned char *)argv[j]);
                if (ret)
                {
                    break;
                }
            }
            else if (i == 2)
            {
                ret = config_b_flags(&b_flags, (unsigned char *)argv[j]);
                if (ret)
                {
                    break;
                }
            }
            else if (i == 3)
            {
                ret = config_c_flags(&c_flags, (unsigned char *)argv[j]);
                if (ret)
                {
                    break;
                }
            }
            else if (i == 4)
            {
                ret = config_err_cnt(&err_cnt, (unsigned char *)argv[j]);
                if (ret)
                {
                    break;
                }
            }
        }
    }

    rfp = fopen("/dev/mmcblk0p2", "rb");
    if (rfp == NULL)
    {
        printf("can not open /dev/mmcblk0p2\n");
        return 0;
    }

    argvdata = malloc(2*1024+64);
    if (argvdata == NULL)
    {
        printf("memory leak\n");
        return 0;
    }
    memset(argvdata, 0, 2*1024+64);
    pbootargv = (boot_argv_def *)argvdata;
    length = fread(argvdata, 1, 2048, rfp);
    if (length)
    {
        FILE *fp = NULL;

        fp = fopen("/tmp/startinfo", "wb");
        if (fp)
        {
            fprintf(fp, "0x%x, %d, %d, %d, %d, %d\n",
                pbootargv->crc_flag,
                pbootargv->flag,
                pbootargv->a_flags,
                pbootargv->b_flags,
                pbootargv->c_flags,
                pbootargv->err_cnt);

            printf("0x%x, %d, %d, %d, %d, %d\n",
            pbootargv->crc_flag,
            pbootargv->flag,
            pbootargv->a_flags,
            pbootargv->b_flags,
            pbootargv->c_flags,
            pbootargv->err_cnt);

            fclose(fp);
        }
    }

    //pbootargv->data = argvdata+4+5;

//    length = get_bootm_argc_length(pbootargv->data);
//    printf("length = %d\n", length);

    fclose(rfp);

    if (flags != 255)
    {
        pbootargv->flag = flags;
        bsave = 1;
    }

    if (a_flags != 255)
    {
        pbootargv->a_flags = a_flags;
        bsave = 1;
    }

    if (b_flags != 255)
    {
        pbootargv->b_flags = b_flags;
        bsave = 1;
    }

    if (c_flags != 255)
    {
        pbootargv->c_flags = c_flags;
        bsave = 1;
    }

    if (err_cnt != 255)
    {
        pbootargv->err_cnt = err_cnt;
        bsave = 1;
    }

    if (bsave == 1)
    {
        FILE *wfp = NULL;

        wfp = fopen("/dev/mmcblk0p2", "wb");
        if (wfp == NULL)
        {
            return 0;
        }

        fwrite(argvdata, 1, 2048, wfp);
        fclose(wfp);

    }

    free(argvdata);
    return 1;
}

#endif

#if 1
int main( int argc, char **argv )
{
    FILE *rfp = NULL;
    int length;
    unsigned int crc_value;
    unsigned int uboot_crc;

    unsigned char flags = 0;
    unsigned char a_flags = 0;
    unsigned char b_flags = 0;
    unsigned char c_flags = 0;
    unsigned char cnt_err = 0;

    unsigned char *argvdata = NULL;

    //printf("hello world\n");

    rfp = fopen("/dev/mmcblk0p2", "rb");
    if (rfp == NULL)
    {
        printf("can not open /dev/mmcblk0p2\n");
        return 0;
    }

    argvdata = malloc(512*1024);
    if (argvdata == NULL)
    {
        printf("memory leak\n");
        return 0;
    }
    memset(argvdata, 0, 512*1024);

    length = fread(&uboot_crc, 1, 4, rfp);
    if (length)
    {
        printf("uboot_crc %x\n", uboot_crc);
    }

    fread(&flags, 1, 1, rfp);
    fread(&a_flags, 1, 1, rfp);
    fread(&b_flags, 1, 1, rfp);
    fread(&c_flags, 1, 1, rfp);
    fread(&cnt_err, 1, 1, rfp);

    length = fread(argvdata, 1, 65527, rfp);

    length = get_bootm_argc_length(argvdata);
    printf("length = %d\n", length);

    crc_value = crc32_le(0, argvdata, 65527);
    printf("crc %x\n", crc_value);


    fclose(rfp);

    return 1;
}
#endif

