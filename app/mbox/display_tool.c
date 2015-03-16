#include "sys_include.h"
#include "img_public.h"
#include "temperature.h"

void prepare_bg(void)
{
    Pixel64 *pCanvas;
    int width;
    int height;
    GrPos Pos;
    GrRegion Region;

    decJpgFile("/tmp/res/bg_product.jpg", &pCanvas, &height, &width);
    Pos.x = Pos.y = 0;
    Region.x = Region.y = 0;
    Region.w = width;
    Region.h = height;

    paint_canvas_2_background(pCanvas, Pos, Region);

    refresh_background_2_device(Pos, Region);

    return ;
}

static struct timeval record_tm;
static struct timeval rtc_record_tm;

static struct timeval ai_record_tm;
void init_temperature_module(void)
{
    gettimeofday(&(record_tm), NULL);
    gettimeofday(&(rtc_record_tm), NULL);
    gettimeofday(&(ai_record_tm), NULL);
}

void show_temperature_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;
    float temp;
    char data[32];
    GrPos Pos;
    static GrRegion Region = {0, 0, 500, 40};
    Pixel64 FT;

    gettimeofday(&cur_tm, NULL);

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(record_tm.tv_sec*1000+(record_tm.tv_usec)/1000));
    if (distms >= 1*1000)
    {
        memset(data, 0, 32);
        temp = read_temperature();
        sprintf(data, "temperature %3.4f", temp);

        Pos.x = 590;
        Pos.y = 50;
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;
        Region.x = Region.y = 0;
        Region.w = 400;
        Region.h = 40;
        refresh_background_2_device(Pos, Region);
        ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);

        record_tm = cur_tm;
    }
}

void show_rtc_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;
    float temp;
    char data[64];
    GrPos Pos;
    static GrRegion Region = {0, 0, 500, 40};
    Pixel64 FT;

    gettimeofday(&cur_tm, NULL);

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(rtc_record_tm.tv_sec*1000+(rtc_record_tm.tv_usec)/1000));
    if (distms >= 2*1000)
    {
        sync_time_from_rtc();

        memset(data, 0, 64);
        get_time_info_string(data);
        Pos.x = 590;
        Pos.y = 90;
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;
        Region.x = Region.y = 0;
        Region.w = 400;
        Region.h = 40;
        refresh_background_2_device(Pos, Region);
        ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);

        rtc_record_tm = cur_tm;
    }
}

void ai_display_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;
    float temp;
    char data[32];
    GrPos Pos;
    static GrRegion Region = {0, 0, 500, 40};
    Pixel64 FT;

    gettimeofday(&cur_tm, NULL);

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(ai_record_tm.tv_sec*1000+(ai_record_tm.tv_usec)/1000));
    if (distms >= 10*1000)
    {
        if (access("/tmp/leftai_ok", F_OK) == 0)
        {
            memset(data, 0, 32);
            sprintf(data, "mic输入ok");
            Pos.x = 590;
            Pos.y = 90;
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            Region.x = Region.y = 0;
            Region.w = 400;
            Region.h = 40;

            ft_Font_Str2Disp_return_region(data,
                FT,
                Pos,
                35,
                &Region);
        }

        if (access("/tmp/rightai_ok", F_OK) == 0)
        {
            memset(data, 0, 32);
            sprintf(data, "LINE输入ok");
            Pos.x = 840;
            Pos.y = 90;
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            Region.x = Region.y = 0;
            Region.w = 400;
            Region.h = 40;

            ft_Font_Str2Disp_return_region(data,
                FT,
                Pos,
                35,
                &Region);
        }
        ai_record_tm = cur_tm;
    }
}

void display_welcom(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;
    char data[100];


    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    Pos.x = 520;
    Pos.y = 225;
    display_character_into_screen(WELLCOM_STR,
        FT,
        &Pos, &Region);

    getchar();

}
static int fd_ser = -1;

void series_display(void)
{
    GrPos Pos;
    GrRegion Region;
    int ret;

    Pixel64 FT;
    unsigned char data[100];

    memset(data, 0, 100);
    sprintf(data, "hello COM2 send");

    fd_ser = InitSerialCom(2, 115200, 'n', 1, 8);
    SerialSend(fd_ser, data, 5);

    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    Pos.x = 520;
    Pos.y = 265;

    display_character_into_screen(CONFIRM_SERIES,
        FT,
        &Pos, &Region);

    getchar();
    return ;
}

void display_msata(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = 1;
    Pos.x = 520;
    Pos.y = 305;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    ret = confirm_msata_running();
    if (ret)
    {
        display_character_into_screen(MSATA_OK,
            FT,
            &Pos, &Region);
    }
    else
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        display_character_into_screen(MSATA_FAILED,
            FT,
            &Pos, &Region);
    }

    getchar();
}

void display_tfcard(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = confirm_tfcard_running();
    Pos.x = 520;
    Pos.y = 345;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    ret = confirm_tfcard_running();

    if (ret)
    {
        display_character_into_screen(TFCARD_OK,
            FT,
            &Pos, &Region);
    }
    else
    {

        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;

        display_character_into_screen(TFCARD_FAILED,
            FT,
            &Pos, &Region);
    }

    getchar();
}

void display_info(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    int id_length;
    int mac_length;

    char stb_id[64];
    char mac_id[64];

    Pos.x = 520;
    Pos.y = 385;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    memset(stb_id, 0, 64);
    memset(mac_id, 0, 64);
    display_character_into_screen(INFO_INPUT,
        FT,
        &Pos, &Region);

    while (1)
    {
        printf("input pcba sn:");
        scanf("%s", stb_id);
        printf("input pcba mac:");
        scanf("%s", mac_id);

        id_length = strlen(stb_id);
        mac_length = strlen(mac_id);

        M_TRACE(DEBUG_TRACE, SYS_MODE, "id = %d, mac = %d\n", id_length, mac_length);
        if ((id_length == 24) &&
            (mac_length == 17))
        {
            break;
        }

        memset(stb_id, 0, 64);
        memset(mac_id, 0, 64);
    }


    /* 刘工bootm 怎么用*/
    set_pcba_mac(mac_id);
    set_pcba_sn(stb_id);
}

void display_stb_info(void)
{
    char stb_id[64];
    char mac_id[64];

    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;
    char data[100];
    //get stbinfo

    memset(stb_id, 0, 64);
    memset(mac_id, 0, 64);

    get_pcba_mac(mac_id, 64);
    get_pcba_sn(stb_id, 64);
    //sprintf(stb_id, "0100000000000001");
    //sprintf(mac_id, "01:23:45:67:89:00");

    memset(data, 0, 100);
    Pos.x = 50;
    Pos.y = 170;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    sprintf(data, "SN:%s", stb_id);
    ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);

    memset(data, 0, 100);
    Pos.x = 50;
    Pos.y = 220;

    sprintf(data, "MAC:%s", mac_id);
    ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);
}

void display_player(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    Pos.x = 520;
    Pos.y = 425;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    display_character_into_screen(VIDEO_CONFIRM,
        FT,
        &Pos, &Region);
    setFramebufAlpha((unsigned char ) 128);
    system("/tmp/player /tmp/160001.MPG");
}

