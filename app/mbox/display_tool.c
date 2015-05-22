#include "sys_include.h"
#include "img_public.h"
#include "temperature.h"

#define wel_pos_x  520
#define wel_pos_y  225

#define mute_pos_x  520
#define mute_pos_y  265

#define ser_pos_x  520
#define ser_pos_y  305

#define voltage_pos_x 520
#define voltage_pos_y 345

#define msata_pos_x 520
#define msata_pos_y 385

#define tfc_pos_x  520
#define tfc_pos_y  545

#define usb_pos_x  520
#define usb_pos_y  585

#define register_pos_x 520
#define register_pos_y 425

#define gpio_pos_x 520
#define gpio_pos_y 465

#define gps_pos_x 520
#define gps_pos_y 505

#define play_pos_x 520
#define play_pos_y 545

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
static struct timeval ai_finish_tm;
static int bquit = 0;

void init_temperature_module(void)
{
    gettimeofday(&(record_tm), NULL);
    gettimeofday(&(rtc_record_tm), NULL);
    gettimeofday(&(ai_record_tm), NULL);
    gettimeofday(&(ai_finish_tm), NULL);
}

void show_temperature_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;
    float temp;
    char data[64];
    GrPos Pos;
    static GrRegion Region = {0, 0, 500, 40};
    Pixel64 FT;
    static int tmperature_cnt = 3;

    gettimeofday(&cur_tm, NULL);

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(record_tm.tv_sec*1000+(record_tm.tv_usec)/1000));
    if (distms >= 1*1000)
    {
        memset(data, 0, 64);
        temp = read_temperature();
        if ((temp <= 25) ||
            (temp >= 60))
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 0;
            FT.RGB.b = 0;

#if 0
            set_test_status(temperature_failure);
            if (tmperature_cnt == 3)
            {
                printf("\ntemperature failure\n");
            }
            tmperature_cnt++;
#endif
        }
        else
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            if (tmperature_cnt == 3)
            {
                printf("\ntemperature success\n");
            }
            tmperature_cnt ++;

            set_temperature_success();
        }

        if (tmperature_cnt > 4)
        {
            tmperature_cnt = 4;
        }

        if (tmperature_cnt == 3)
        {
            printf("temperature %3.4fC\n", temp);
        }

        sprintf(data, "temperature %3.4fC", temp);

        Pos.x = 590;
        Pos.y = 50;

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
    static int rtc_cnt = 0;

    gettimeofday(&cur_tm, NULL);


    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(rtc_record_tm.tv_sec*1000+(rtc_record_tm.tv_usec)/1000));
    if (distms >= 2*1000)
    {
        //printf("11111111\n");
        rtc_cnt ++;
        if (rtc_cnt > 4)
        {
            rtc_cnt = 4;
        }

        ret = sync_time_from_rtc();
        if (ret == 0)
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 0;
            FT.RGB.b = 0;
            if (rtc_cnt == 3)
            {
                printf("\nrtc failure [sync_time_from_rtc]\n");
                set_test_status(rtc_failure);
            }

        }
        else
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
        }

        memset(data, 0, 64);
        ret = get_time_info_string(data);
        if (ret == 0)
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 0;
            FT.RGB.b = 0;
            if (rtc_cnt == 3)
            {
                printf("\nrtc failure %s\n", data);
                set_test_status(rtc_failure);
            }

        }
        else
        {
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            if (rtc_cnt == 3)
            {
                printf("\nrtc ok|%s\n", data);
            }
        }

        Pos.x = 590;
        Pos.y = 90;

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

int get_ai_force_exit(void)
{
    return bquit;
}

void ai_display_filter(int flag)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;
    float temp;
    char data[32];
    GrPos Pos;
    static GrRegion Region = {0, 0, 500, 40};
    Pixel64 FT;

    static int line_ok_disp = 0;
    static int mic_ok_disp = 0;

    if (flag == 0)
    {
        if (bquit)
        {
            return ;
        }

        gettimeofday(&cur_tm, NULL);

        distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(ai_finish_tm.tv_sec*1000+(ai_finish_tm.tv_usec)/1000));
        if (distms >= 60*1000)
        {
            bquit = 1;

            if (access("/tmp/rightai_ok", F_OK) == 0)
            {
                memset(data, 0, 32);
                sprintf(data, "mic输入ok");
                Pos.x = 590;
                Pos.y = 130;
                FT.RGB.a = 255;
                FT.RGB.r = 255;
                FT.RGB.g = 255;
                FT.RGB.b = 255;
                Region.x = Region.y = 0;
                Region.w = 240;
                Region.h = 40;
                if (mic_ok_disp == 0)
                {
                    mic_ok_disp = 1;
                    printf("\nmic success\n");

                refresh_background_2_device(Pos, Region);
                ft_Font_Str2Disp_return_region(data,
                    FT,
                    Pos,
                    35,
                    &Region);
                }
            }
            else
            {
                printf("\nmic failure\n");
            }

            if (access("/tmp/leftai_ok", F_OK) == 0)
            {
                memset(data, 0, 32);
                sprintf(data, "LINE输入ok");
                Pos.x = 840;
                Pos.y = 130;
                FT.RGB.a = 255;
                FT.RGB.r = 255;
                FT.RGB.g = 255;
                FT.RGB.b = 255;
                Region.x = Region.y = 0;
                Region.w = 200;
                Region.h = 40;

                if (line_ok_disp == 0)
                {
                    printf("\nline success\n");
                    refresh_background_2_device(Pos, Region);
                    ft_Font_Str2Disp_return_region(data,
                        FT,
                        Pos,
                        35,
                        &Region);
                    line_ok_disp = 1;
                }
            }
            else
            {
                printf("\nline failure\n");
            }

            return ;
        }

    }

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(ai_record_tm.tv_sec*1000+(ai_record_tm.tv_usec)/1000));
    if (flag == 1)
    {
        mic_ok_disp = 0;
        line_ok_disp = 0;
        distms = 6000;
        printf("force display\n");
    }

    if (distms >= 5*1000)
    {
        //printf("sssssssssssssssss\n");
        if (access("/tmp/rightai_ok", F_OK) == 0)
        {
            //printf("aaaaaaaaaaaaaaaaaaaa\n");
            memset(data, 0, 32);
            sprintf(data, "LINE输入ok");
            Pos.x = 590;
            Pos.y = 130;
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            Region.x = Region.y = 0;
            Region.w = 240;
            Region.h = 40;

            if (mic_ok_disp == 0)
            {
                mic_ok_disp = 1;

                printf("\nline success\n");
                refresh_background_2_device(Pos, Region);
                ft_Font_Str2Disp_return_region(data,
                    FT,
                    Pos,
                    35,
                    &Region);

            }
        }

       /*if (access("/tmp/leftai_ok", F_OK) == 0)
	{
            memset(data, 0, 32);
            sprintf(data, "MIC输入ok");
            Pos.x = 840;
            Pos.y = 130;
            FT.RGB.a = 255;
            FT.RGB.r = 255;
            FT.RGB.g = 255;
            FT.RGB.b = 255;
            Region.x = Region.y = 0;
            Region.w = 200;
            Region.h = 40;

            if (line_ok_disp == 0)
            {
                line_ok_disp = 1;
                printf("\nmic success\n");
                refresh_background_2_device(Pos, Region);
                ft_Font_Str2Disp_return_region(data,
                    FT,
                    Pos,
                    35,
                    &Region);
            }
        }*/

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

    Pos.x = wel_pos_x;
    Pos.y = wel_pos_y;
    display_character_into_screen(WELLCOM_STR,
        FT,
        &Pos, &Region);

    Pos.x = 590;
    Pos.y = 130;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 0;
    FT.RGB.b = 0;
    Region.x = Region.y = 0;
    Region.w = 400;
    Region.h = 40;

    //refresh_background_2_device(Pos, Region);
   /* ft_Font_Str2Disp_return_region("请输入mic",
        FT,
        Pos,
        35,
        &Region);*/


    ft_Font_Str2Disp_return_region("输入linein",
        FT,
        Pos,
        35,
        &Region);

	Pos.x = 590;
    Pos.y = 130;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 0;
    FT.RGB.b = 0;
    Region.x = Region.y = 0;
    Region.w = 200;
    Region.h = 40;

}

void mute_ai_display(void)
{
    GrPos Pos;
    GrRegion Region;
    int ret;

    Pixel64 FT;


    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    Pos.x = mute_pos_x;
    Pos.y = mute_pos_y;

   /* display_character_into_screen(MUTE_MIC_TEST,
        FT,
        &Pos, &Region);*/


    return ;
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
    sprintf(data, "hello UART3 send");

    fd_ser = InitSerialCom(4, 115200, 'n', 1, 8);
    SerialSend(fd_ser, data, 5);

    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    Pos.x = ser_pos_x;
    Pos.y = ser_pos_y;

    display_character_into_screen(CONFIRM_SERIES,
        FT,
        &Pos, &Region);

//    getchar();
    return ;
}

void display_msata(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = 1;
    Pos.x = msata_pos_x;
    Pos.y = msata_pos_y;
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

//    getchar();
}

void display_tfcard(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = confirm_tfcard_running();
    Pos.x = tfc_pos_x;
    Pos.y = tfc_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    //ret = confirm_tfcard_running();

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
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        display_character_into_screen(TFCARD_FAILED,
            FT,
            &Pos, &Region);
    }

//    getchar();
}

void display_usb(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = confirm_usb_running();
    Pos.x = usb_pos_x;
    Pos.y = usb_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    if (ret)
    {
        display_character_into_screen(USB_OK,
            FT,
            &Pos, &Region);
    }
    else
    {

        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        display_character_into_screen(USB_FAILED,
            FT,
            &Pos, &Region);
    }

//    getchar();
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
	char iccid[64];

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
	get_pcba_iccid(iccid,64);
    //sprintf(stb_id, "0100000000000001");
    //sprintf(mac_id, "01:23:45:67:89:00");

    memset(data, 0, 100);
    Pos.x = 50;
    Pos.y = 170;

    if (strlen(stb_id) < 21)
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        sprintf(data, "SN:NULL");

        printf("\nsn read failure\n");
    }
    else
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;

        sprintf(data, "SN:%s", stb_id);

        printf("\nsn read ok |%s\n", stb_id);
    }

    Region.x = Region.y = 0;
    Region.w = 500;
    Region.h = 32;
    refresh_background_2_device(Pos, Region);
    ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);

    memset(data, 0, 100);

    if (strlen(mac_id) < 12)
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        sprintf(data, "MAC:NULL");

        printf("\nmac read failure\n");
    }
    else
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;

        sprintf(data, "MAC:%s", mac_id);

        printf("\nmac read ok |%s\n", mac_id);

    }
    Pos.x = 50;
    Pos.y = 220;

    Region.x = Region.y = 0;
    Region.w = 400;
    Region.h = 35;
    refresh_background_2_device(Pos, Region);

    //sprintf(data, "MAC:%s", mac_id);
    ft_Font_Str2Disp_return_region(data,
            FT,
            Pos,
            35,
            &Region);

	if (strlen(iccid) < 21)
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        sprintf(data, "ICCID:NULL");

        printf("\niccid read failure\n");
    }
    else
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 255;
        FT.RGB.b = 255;

        sprintf(data, "ICCID:%s", iccid);

        printf("\niccid read ok |%s\n", iccid);

    }
    Pos.x = 50;
    Pos.y = 270;

    Region.x = Region.y = 0;
    Region.w = 400;
    Region.h = 35;
    refresh_background_2_device(Pos, Region);

    //sprintf(data, "MAC:%s", mac_id);
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
    Pos.x = play_pos_x;
    Pos.y = play_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    display_character_into_screen(VIDEO_CONFIRM,
        FT,
        &Pos, &Region);
    setFramebufAlpha((unsigned char ) 128);
    system("/tmp/player /tmp/160001.MPG >/tmp/play.log");
}

void display_gpio_putdown(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    Pos.x = gpio_pos_x;
    Pos.y = gpio_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 0;
    FT.RGB.b = 0;

    Region.x = Region.y = 0;
    Region.w = 400;
    Region.h = 35;
    refresh_background_2_device(Pos, Region);

    display_character_into_screen(GPIO_PUTDOWN,
        FT,
        &Pos, &Region);

}

void display_gpio_test(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    Pos.x = gpio_pos_x;
    Pos.y = gpio_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    Region.x = Region.y = 0;
    Region.w = 400;
    Region.h = 35;
    refresh_background_2_device(Pos, Region);

    display_character_into_screen(GPIO_TEST,
        FT,
        &Pos, &Region);

}

void display_register(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret;

    ret = 1;
    Pos.x = register_pos_x;
    Pos.y = register_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

	FILE *pf = popen("cat /tmp/registerResult.txt | awk -F ':' '{print $2}'| awk -F '}' '{print $1}'","r");
	char res[16]={0};
	fread(res, 16 , 1,pf);
	ret = atoi(res);
    //ret = confirm_register();
    if (ret == 0)
    {
        display_character_into_screen(register_OK,
            FT,
            &Pos, &Region);
    }
    else
    {
        FT.RGB.a = 255;
        FT.RGB.r = 255;
        FT.RGB.g = 0;
        FT.RGB.b = 0;

        display_character_into_screen(register_FAILED,
            FT,
            &Pos, &Region);
    }
	pclose(pf);
}

void display_gps(void)
{
    Pixel64 FT;

    GrPos Pos;
    GrRegion Region;
    int ret,timing;
	
	char res[16] = {0};
	char longitude[16]={0};
	char latitude[16]={0};

    //ret = 1;
    Pos.x = gps_pos_x;
    Pos.y = gps_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

	for(timing = 1;timing <= 12;timing++)
	{
		//ret = confirm_gps();
		FILE *pf = popen("cat /tmp/.gps/status","r");
		
		fread(res,16,1,pf);
		ret = atoi(res);
		printf("ret=%d",ret);
    	if (ret == 1)
    	{
			FILE *p_Lng=popen("cat /tmp/.gps/gps_Lng","r");
			fread(longitude,16,1,p_Lng);

			FILE *p_Lat=popen("cat /tmp/.gps/gps_Lat","r");
			fread(latitude,16,1,p_Lat);
			
			display_character_into_screen(gps_OK,
            	FT,
            	&Pos, &Region);

			Pos.x = 800;
			ft_Font_Str2Disp_return_region(longitude,
            	FT,
            	Pos,
            	35,
            	&Region);

			Pos.x = 1050;
			ft_Font_Str2Disp_return_region(latitude,
            	FT,
            	Pos,
            	35,
            	&Region);
			
			pclose(p_Lng);
			pclose(p_Lat);
			break;
    	}
    	else
    	{       	
			if(timing = 12)
			{
				FT.RGB.a = 255;
    			FT.RGB.r = 255;
    			FT.RGB.g = 0;
    			FT.RGB.b = 0;

    			display_character_into_screen(gps_FAILED,
        			FT,
        			&Pos, &Region);
				break;
			}
		
		}
		
		sleep(500*1000);
		pclose(pf);
		printf("%d\n",timing);
	}
	
}

void display_voltage(void)             //电压测试
{
	Pixel64 FT;
    GrPos Pos;
    GrRegion Region;

    Pos.x = voltage_pos_x;
    Pos.y = voltage_pos_y;
    FT.RGB.a = 255;
    FT.RGB.r = 255;
    FT.RGB.g = 255;
    FT.RGB.b = 255;

    float voltage;
    FILE *pf = popen("voltage_show | awk -F ':' '{print $2}'","r");
    char res[32]={0};
	fread(res,32,1,pf);
	voltage=atof(res);
	printf("%f",voltage);    

	if((voltage > 23.7 & voltage <24.3)||(voltage > 11.7 & voltage < 12.3) )
	{
		display_character_into_screen(voltage_OK,
            FT,
            &Pos, &Region);	                                              //电压正常
		Pos.x = 750;
		ft_Font_Str2Disp_return_region(res,
            FT,
            Pos,
            35,
            &Region);
	}
	else
	{
		if(voltage <23.7 || voltage < 11.7 )
		{
			FT.RGB.a = 255;
    		FT.RGB.r = 255;
    		FT.RGB.g = 0;
    		FT.RGB.b = 0;

			display_character_into_screen(voltage_FAILED_L,
            	FT,
            	&Pos, &Region);                                           //电压偏小
			Pos.x = 750;
		ft_Font_Str2Disp_return_region(res,
            FT,
            Pos,
            35,
            &Region);
		}
		else
		{
			FT.RGB.a = 255;
    		FT.RGB.r = 255;
    		FT.RGB.g = 0;
    		FT.RGB.b = 0;

			display_character_into_screen(voltage_FAILED_H,
            	FT,
            	&Pos, &Region);                                            //电压偏高
			Pos.x = 750;
		ft_Font_Str2Disp_return_region(res,
            FT,
            Pos,
            35,
            &Region);
		}
	}
	pclose(pf);
}

//    getchar();

