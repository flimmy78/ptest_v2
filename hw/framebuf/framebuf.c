#include "sys_include.h"

#include "framebuf_public.h"

static Pixel64 *pFramebufCanvas = NULL;
static int gr_w = 1280;
static int gr_h = 720;
/*
    初始化frame buffer模块
*/
int init_framebuf_module(void)
{
    const char *fbname = "/dev/fb0";

    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    char * fb_base_addr = NULL;
    unsigned int screensize;
    int PAGE_SIZE;
    int g_fbdev;

    if (pFramebufCanvas != NULL)
    {
        fprintf(stderr, "frame buffer has been initialized\r\n");
        return 1;
    }

    //打开framebuffer设备
    g_fbdev = open(fbname, O_RDWR);
    if( g_fbdev < 0)
    {
        fprintf(stderr, "%s, %d\r\n", __FILE__, __LINE__);
        return 0;
    }

    if (-1 == ioctl(g_fbdev, FBIOGET_VSCREENINFO,&fb_var))
    {
        fprintf(stderr, "%s, %d\r\n", __FILE__, __LINE__);
        return 0;
    }
    if (-1 == ioctl(g_fbdev, FBIOGET_FSCREENINFO,&fb_fix))
    {
        fprintf(stderr, "%s, %d\r\n", __FILE__, __LINE__);
        return 0;
    }

	screensize = fb_var.xres * fb_var.yres * fb_var.bits_per_pixel / 8;

    fprintf(stderr, "x = %d, y = %d, pixel bit = %d screensize = %d\r\n",
        fb_var.xres, fb_var.yres, fb_var.bits_per_pixel, screensize);

    gr_w = fb_var.xres;
    gr_h = fb_var.yres;

    //获取framebuffer的可操作内存地址map
    PAGE_SIZE = sysconf(_SC_PAGESIZE);
    fb_base_addr = (char *)mmap(NULL , screensize+PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, g_fbdev, 0);

    // temporary fix for 5159, mapping is paged aligned
    if (fb_fix.smem_start & (PAGE_SIZE-1))
    {
        fb_base_addr += (fb_fix.smem_start & (PAGE_SIZE-1));
        fprintf(stderr, "Fix alignment 0x%08lx -> %p.\n",
                fb_fix.smem_start, fb_base_addr);
    }

    pFramebufCanvas = (Pixel64 *) fb_base_addr;
    init_logic_framebuf_module();

	return 1;
}

/*
    获取frame buffer的地址
*/
Pixel64 *getFramebuf(void)
{
    return pFramebufCanvas;
}

/*
    清空图像层
*/
void clearFramebuf(void)
{
    memset(pFramebufCanvas, 0, (gr_w*gr_h*4));
}

void test_framebuf(void)
{
    static int s_cnt = 0;
    Pixel64 *pCanvas = NULL;
    int i, j;

    fprintf(stderr, "s_cnt = %d\r\n", s_cnt);
    pCanvas = (Pixel64 *) getFramebuf();

    for (i = 0; i < gr_h; i++)
    {
        for (j = 0; j < gr_w; j++)
        {
            if (s_cnt == 0)
            {
                pCanvas->RGB.a = 255;
                pCanvas->RGB.r = 0;
                pCanvas->RGB.g = 0;
                pCanvas->RGB.b = 255;

                pCanvas ++;
            }
            else if (s_cnt == 1)
            {
                pCanvas->RGB.a = 255;
                pCanvas->RGB.r = 0;
                pCanvas->RGB.g = 255;
                pCanvas->RGB.b = 0;
                pCanvas ++;
            }
            else if (s_cnt == 2)
            {
                pCanvas->RGB.a = 255;
                pCanvas->RGB.r = 255;
                pCanvas->RGB.g = 0;
                pCanvas->RGB.b = 0;
                pCanvas ++;
            }
        }
    }

    if (s_cnt == 0)
    {
        s_cnt = 1;
    }
    else if (s_cnt == 1)
    {
        s_cnt = 2;
    }
    else
    {
        s_cnt = 0;
    }


    return ;

}

/*
    设定图像层的透明度
*/
void setFramebufAlpha(unsigned char alpha)
{
    int i, j;

    Pixel64 *pCanvas = NULL;

    pCanvas = getFramebuf();

    for (i = 0; i < gr_h; i++)
    {
        for (j = 0; j < gr_w; j++)
        {
            pCanvas->RGB.a = alpha;
            pCanvas ++;
        }
    }
}

int get_framebuf_width(void)
{
    return gr_w;
}

int get_framebuf_height(void)
{
    return gr_h;
}

