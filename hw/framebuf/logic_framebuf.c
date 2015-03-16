
#include "sys_include.h"
#include "framebuf_public.h"

static Pixel64 *pBgCanvas = NULL;
static Pixel64 *pLogicCanvas = NULL;
static int gr_w = 1280;
static int gr_h = 720;

//将一个像素合并到设备里面或者一个已知图层里面
int merge_one_pixel_2_device(Pixel64 *pDevice, Pixel64 *pCanvas)
{
    unsigned char a;
    unsigned char b;

    if (pCanvas->RGB.a)
    {
        a = pCanvas->RGB.a;
        b = 255 - a;
        pDevice->RGB.r = ((pCanvas->RGB.r*a)>>8) + ((pDevice->RGB.r*b)>>8);
        pDevice->RGB.g = ((pCanvas->RGB.g*a)>>8) + ((pDevice->RGB.g*b)>>8);
        pDevice->RGB.b = ((pCanvas->RGB.b*a)>>8) + ((pDevice->RGB.b*b)>>8);
    }

    return 1;
}

int refresh_one_pixel_2_main_display(Pixel64 *pDevice, GrPos Pos)
{
    Pixel64 *pCanvas;

    pCanvas = getFramebuf();
    pCanvas += (Pos.x + Pos.y*gr_w);

    pCanvas->value32 = pDevice->value32;

    return 1;
}


//将两个像素合并到一个新的画布上
//surface是参考的背景画面像素点
int merge_two_pixel_2_canvas(Pixel64 *pSurface, Pixel64 *pCanvas, Pixel64 *pDevice)
{
    unsigned char a;
    unsigned char b;

    a = pCanvas->RGB.a;
    b = 255 - a;

    pDevice->RGB.a = 255;
    pDevice->RGB.r = ((pCanvas->RGB.r*a)>>8) + ((pSurface->RGB.r*b)>>8);
    pDevice->RGB.g = ((pCanvas->RGB.g*a)>>8) + ((pSurface->RGB.g*b)>>8);
    pDevice->RGB.b = ((pCanvas->RGB.b*a)>>8) + ((pSurface->RGB.b*b)>>8);

    return 1;
}

int init_logic_framebuf_module(void)
{

    gr_w = get_framebuf_width();
    gr_h = get_framebuf_height();

    if (pBgCanvas == NULL)
    {
        pBgCanvas = malloc(gr_w*gr_h*4);
    }

    if (pLogicCanvas == NULL)
    {
        pLogicCanvas = malloc(gr_w*gr_h*4);
    }

    return 1;
}

int close_logic_framebuf_module(void)
{
    if (pBgCanvas)
    {
        free(pBgCanvas);
        pBgCanvas = NULL;
    }

    if (pLogicCanvas)
    {
        free(pLogicCanvas);
        pLogicCanvas = NULL;
    }

    return 1;
}

Pixel64 *get_background_canvas(void)
{
    return pBgCanvas;
}

Pixel64 *get_logic_canvas(void)
{
    return pLogicCanvas;
}

//Region 是画布canvas的矩形规格
//Pos 代表画布平铺在设备framebuffer上的起点位置
void paint_canvas_2_device(Pixel64 *pCanvas, GrPos Pos, GrRegion Region)
{
    int i, j;
    Pixel64 *pDevice = NULL;

    pDevice = getFramebuf();
    pDevice += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            pDevice[j].value32 = pCanvas[j].value32;
        }

        pDevice += gr_w;
        pCanvas += Region.w;
    }
    return ;
}

void paint_canvas_2_background(Pixel64 *pCanvas, GrPos Pos, GrRegion Region)
{
    int i, j;
    Pixel64 *pDevice = NULL;

    pDevice = get_background_canvas();
    pDevice += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            pDevice[j].value32 = pCanvas[j].value32;
        }

        pDevice += gr_w;
        pCanvas += Region.w;
    }
    return ;
}

void paint_canvas_2_logic_device(Pixel64 *pCanvas, GrPos Pos, GrRegion Region)
{
    int i, j;
    Pixel64 *pDevice = NULL;

    pDevice = get_logic_canvas();
    pDevice += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            pDevice[j].value32 = pCanvas[j].value32;
        }

        pDevice += gr_w;
        pCanvas += Region.w;
    }
    return ;
}

void refresh_background_2_device(GrPos Pos, GrRegion Region)
{
    int i, j;
    Pixel64 *pDevice = NULL;
    Pixel64 *pCanvas = NULL;

    pDevice = getFramebuf();
    pDevice += (Pos.x + Pos.y*gr_w);

    pCanvas = get_background_canvas();
    pCanvas += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            pDevice[j].value32 = pCanvas[j].value32;
        }

        pDevice += gr_w;
        pCanvas += gr_w;
    }
    return ;
}

void refresh_logic_2_device(GrPos Pos, GrRegion Region)
{
    int i, j;
    Pixel64 *pDevice = NULL;
    Pixel64 *pCanvas = NULL;

    pDevice = getFramebuf();
    pDevice += (Pos.x + Pos.y*gr_w);

    pCanvas = get_logic_canvas();
    pCanvas += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            pDevice[j].value32 = pCanvas[j].value32;
        }

        pDevice += gr_w;
        pCanvas += gr_w;
    }
    return ;
}

//将两个矩形区域混叠拷贝到第三方区域
int merge_two_region_2_device(Pixel64 *pSurface,
                Pixel64 *pCanvas,
                Pixel64 *pDevice,
                GrPos Pos, GrRegion Region)
{
    int i, j;

    pSurface += (Pos.x + Pos.y*gr_w);
    pCanvas  += (Pos.x + Pos.y*gr_w);
    pDevice  += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        for (j = 0; j < Region.w; j++)
        {
            merge_two_pixel_2_canvas(pSurface+j, pCanvas+j, pDevice+j);
        }

        pSurface += gr_w;
        pDevice += gr_w;
        pCanvas += gr_w;
    }

    return 1;
}

int move_region_2_display(Pixel64 *pCanvas, GrPos Pos, GrRegion Region)
{
    Pixel64 *pDevice = NULL;
    int i;

    pDevice = getFramebuf();

    pDevice += (Pos.x + Pos.y*gr_w);
    pCanvas += (Pos.x + Pos.y*gr_w);

    for (i = 0; i < Region.h; i++)
    {
        memcpy(pDevice, pCanvas, (Region.w*4));
        pCanvas += gr_w;
        pDevice += gr_w;
    }

    return 1;
}


