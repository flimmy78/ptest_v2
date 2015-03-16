#ifndef _FT_FONT_H
#define _FT_FONT_H

#include "sys_include.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FT_MEMORY_LEN	1280*720*2

typedef struct
{
	unsigned char *pmem; //内存起至块
	unsigned long int cost_mem_width;	/*内存块宽度*/
	unsigned long int disp_mem_width;	/*内存中字形位图宽度*/
	unsigned long int disp_mem_height;	/*内存中字形位图高度*/

}FT_MEM_BLOCK;;

int ft_Font_Init(void);
void ft_Font_Destroy(void);

int ft_Font_StringToMem(char *pText, FT_MEM_BLOCK *pMem, unsigned int fontSize, unsigned long int mem_max_len);
int ft_Font_Unicode_ToMem(short *pText,
            FT_MEM_BLOCK *pMem,
            unsigned int fontSize,
            unsigned long int mem_max_len,
            int uncode_len);

int ft_Font_Unicode_ToMem_with_width(short *pText,
        FT_MEM_BLOCK *pMem,
        unsigned int fontSize,
        unsigned long int mem_max_len,
        int uncode_len,
        int width,
        int *pout_len);


void ft_Font_Draw_2_Canvas(FT_MEM_BLOCK *pMem,
			Pixel64 *pCanvas,
			Pixel64 FT,
			GrPos Pos);

FT_MEM_BLOCK * ft_Font_Get_Default_pmem(void);

int ft_Font_Str2Disp(char *str, Pixel64 FT, GrPos pos, int size);

int ft_Font_Str2Disp_return_region(char *str,
            Pixel64 FT,
            GrPos pos,
            int size,
            GrRegion *pRegion);

int FontConvTxtStr_2_Unicode(char* pText,
            unsigned short **ppDst,
            int *pLenDst);

int ft_Font_Conv_Unicode_Str(char* pSrc,
            int lenSrc,
            unsigned char *pDst,
            int LenDst);

int ft_Font_Str2Canvas(char *str,
            Pixel64 FT,
            Pixel64 *pCanvas,
            GrPos Pos,
            int size);

int ft_Font_Str2Canvas_return_region(char *str,
            Pixel64 FT,
            Pixel64 *pCanvas,
            GrPos Pos,
            int size,
            GrRegion *pRegion);

int ft_Font_Running(void);

void ft_Font_Draw_2_Canvas_with_region(FT_MEM_BLOCK *pMem,
            Pixel64 *pCanvas,
            Pixel64 FT,
            GrPos Pos,
            GrRegion Region);

int ft_Font_Unicode_ToMem_with_const_width(short *pText,
        FT_MEM_BLOCK *pMem,
        unsigned int fontSize,
        unsigned long int mem_max_len,
        int uncode_len,
        int width,
        int *pout_len);

int ft_Font_Str2Disp_with_region(char *str,
        Pixel64 FT,
        GrPos pos,
        int size,
        GrRegion region);


#ifdef __cplusplus
}
#endif

#endif


