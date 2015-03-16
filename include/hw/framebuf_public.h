
#ifndef _FRAMEBUF_PUBLIC_H_
#define _FRAMEBUF_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif


//主端首先应该建立的一个函数，要早于rend对等函数
int init_framebuf_module(void);

//获取一个可用的显存地址
Pixel64 *getFramebuf(void);

//清空图像层
void clearFramebuf(void);

//测试framebuffer
void test_framebuf(void);

//设置alpha
void setFramebufAlpha(unsigned char alpha);

//获取framebuffer的宽度
int get_framebuf_width(void);

//获取framebuffer的高度
int get_framebuf_height(void);

int init_logic_framebuf_module(void);

int close_logic_framebuf_module(void);

Pixel64 *get_background_canvas(void);

Pixel64 *get_logic_canvas(void);

//Region 是画布canvas的矩形规格
//Pos 代表画布平铺在设备framebuffer上的起点位置
void paint_canvas_2_device(Pixel64 *pCanvas, GrPos Pos, GrRegion Region);

void paint_canvas_2_background(Pixel64 *pCanvas, GrPos Pos, GrRegion Region);

void paint_canvas_2_logic_device(Pixel64 *pCanvas, GrPos Pos, GrRegion Region);

void refresh_background_2_device(GrPos Pos, GrRegion Region);

void refresh_logic_2_device(GrPos Pos, GrRegion Region);

//将两个矩形区域混叠拷贝到第三方区域
int merge_two_region_2_device(Pixel64 *pSurface,
                Pixel64 *pCanvas,
                Pixel64 *pDevice,
                GrPos Pos, GrRegion Region);

int move_region_2_display(Pixel64 *pCanvas, GrPos Pos, GrRegion Region);

//将一个像素合并到设备里面或者一个已知图层里面
int merge_one_pixel_2_device(Pixel64 *pDevice, Pixel64 *pCanvas);

int refresh_one_pixel_2_main_display(Pixel64 *pDevice, GrPos Pos);
//将两个像素合并到一个新的画布上
//surface是参考的背景画面像素点
int merge_two_pixel_2_canvas(Pixel64 *pSurface, Pixel64 *pCanvas, Pixel64 *pDevice);

#ifdef __cplusplus
}
#endif

#endif

