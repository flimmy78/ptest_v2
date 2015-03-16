#include "sys_include.h"
#include "display_public.h"
#include "framebuf_public.h"
#include "img_public.h"
#include "ft_Font.h"
#include "temperature.h"
#include "networkPublic.h"

static char stb_id[64];
static char mac_id[64];

static int init_hard_device_task(void);
static int fd_ser = -1;

static int send_data_2_network(void)
{
    udp_item_def Handle;
    char data[100];
    int length;
    int ret = 0;

    memset(data, 0, 100);
    sprintf(data, "hello world!!!");

    Handle.multicast = 0;
    memset(Handle.near_ip, 0, 32);
    memset(Handle.remote_ip, 0, 32);
    Handle.near_port = 8001;
    Handle.remote_port = 8002;
    sprintf(Handle.remote_ip, "192.168.1.104", strlen("192.168.1.104"));

    init_udp_instance(&Handle);
    length = 32;
    ret = send_data_by_udp_instance(&Handle, data, length);
    fprintf(stderr, "%d\n", ret);
    close_udp_instance(&Handle);
    return 1;
}

int main( int argc, char **argv )
{
    char ch;

    //调试输出单元
    tracePrintfInit();
    {
        //modifyTraceLevelByMode(SYS_MODE, DEBUG_TRACE);
        //modifyTraceLevelByMode(IMG_MODE, DEBUG_TRACE);
        //modifyTraceLevelByMode(FT_MODE, DEBUG_TRACE);
    }

    init_display_output_device(FMT_720P_60);

    init_framebuf_module();

    ft_Font_Init();
    init_temperature_module();
    sync_time_from_rtc();

    prepare_bg();
/*
    system("mkdir -p /media");
    system("mkdir -p /media/tfcard");
    system("mkdir -p /media/usb");
    system("mkdir -p /media/msata");
*/
    init_hard_device_task();
    display_stb_info();

    display_welcom();
    series_display();
    display_msata();
    display_tfcard();
    display_info();
    display_stb_info();
    display_player();

#if 1
    while(1)
    {
        ch = getchar();
        if (ch == 'q')
        {
            break;
        }
        else if (ch == 'w')
        {
            test_framebuf();
        }
        else if (ch == 'e')
        {
            clearFramebuf();
        }
        else if (ch == 'r')
        {
            Pixel64 *pCanvas;
            int width;
            int height;
            GrPos Pos;
            GrRegion Region;

            decJpgFile("/nfs/wait4open.jpg", &pCanvas, &height, &width);
            Pos.x = Pos.y = 0;
            Region.x = Region.y = 0;
            Region.w = width;
            Region.h = height;

            paint_canvas_2_background(pCanvas, Pos, Region);

            refresh_background_2_device(Pos, Region);
        }
        else if (ch == 't')
        {
            Pixel64 *pCanvas;
            int width;
            int height;
            GrPos Pos;
            GrRegion Region;
            decPngFile("/nfs/ico_dir.png", &pCanvas, &width, &height);

            Pos.x = 100;
            Pos.y = 100;
            Region.x = 0;
            Region.y = 0;
            Region.w = width;
            Region.h = height;

            paint_canvas_2_logic_device(pCanvas, Pos, Region);
            pCanvas = get_logic_canvas();
            move_region_2_display(pCanvas, Pos, Region);

        }
        else if (ch == 'y')
        {
            GrPos Pos;
            Pixel64 FT;

            Pos.x = 137;
            Pos.y = 102;
            FT.RGB.a = 0xFF;
            FT.RGB.r = 0xD7;
            FT.RGB.g = 0xD7;
            FT.RGB.b = 0xD7;

            ft_Font_Str2Disp("hello world", FT, Pos, 40);
        }
        else if (ch == 'u')
        {
            fprintf(stderr, "开始装备测试\n");
            set_test_network(0);
        }
        else if (ch == 'i')
        {
            confirm_network_running(0);
        }
        else if (ch == 'o')
        {
            float temp;

            temp = read_temperature();
            printf("temperature %3.4f\n", temp);
        }
        else if (ch == 'p')
        {
            save_time_into_rtc();
        }
        else if (ch == 'a')
        {
            sync_time_from_rtc();
        }
        else if (ch == 's')
        {
            int value40;
            int value41;
            int value44;

            read_gpio_status(40, &value40);
            read_gpio_status(41, &value41);
            read_gpio_status(44, &value44);
            printf("40 = %d, 41 = %d, 44 = %d\n", value40, value41, value44);
        }
        else if (ch == 'd')
        {
            write_gpio_status(44, 0);
        }
        else if (ch == 'f')
        {
            write_gpio_status(44, 1);
        }
        else if (ch == 'g')
        {
            fd_ser = InitSerialCom(1,115200,'n',1, 8);
        }
        else if (ch == 'h')
        {
            unsigned char data[10];
            sprintf(data, "hello");
            SerialSend(fd_ser, data, 5);
        }
        else if (ch == 'j')
        {
            unsigned char data[10];
            memset(data, 0, 10);
            serialReceive(fd_ser, data, 10);
            printf("data\n", data);
        }
        else if (ch == 'k')
        {

            send_data_2_network();
        }
        else if (ch == 'l')
        {
            Pixel64 *pcanvas;
            Pixel64 FT;
            int i, j;
            FILE *wfp;
            pcanvas = getFramebuf();

            wfp = fopen("/nfs/screen.raw", "wb");
            for (i = 0; i < 720; i++)
            {
                for (j = 0; j < 1280; j++)
                {

                    FT.RGB.r = pcanvas[j].RGB.r;
                    FT.RGB.b = pcanvas[j].RGB.b;
                    FT.RGB.a = pcanvas[j].RGB.a;
                    FT.RGB.g = pcanvas[j].RGB.g;

                    fwrite(&(FT.RGB.r), 1, 1, wfp);
                    fwrite(&(FT.RGB.g), 1, 1, wfp);
                    fwrite(&(FT.RGB.b), 1, 1, wfp);
                }
                pcanvas += 1280;
            }
            fclose(wfp);
        }

    }
#endif

    close_logic_framebuf_module();
    ft_Font_Destroy();

    close_display_output_device();

    return 0;
}

static int bExitTimerTask = 0;

static pthread_t pthread_timer_task = 0;

//这个任务的时间间隔是50ms
static void* timer_task_func(void *arg)
{
    bExitTimerTask = 1;

    init_gpio_borad();

    while(bExitTimerTask)
    {
        gpio_sharp_filter();
        show_temperature_filter();
        ai_display_filter();
        show_rtc_filter();

        usleep(5*1000);
    }

    return NULL;
}

static int init_hard_device_task(void)
{
    pthread_create(&pthread_timer_task, NULL, timer_task_func, NULL);
    return 1;
}

