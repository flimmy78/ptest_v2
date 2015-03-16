/*! @file
********************************************************************************
<PRE>
模块名       : 字体输出函数,支持turetype的字库显示
文件名       : ft_Font.c
相关文件     : 无
文件实现功能 :

作者         : macros.zhang
版本         : 1.0
--------------------------------------------------------------------------------
备注         : 无
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容
2010/02/22   1.0      macros

这个模块是不可以重入的否则容易造成死机主要是转化字符串带来的问题
</PRE>
*******************************************************************************/

#include "ft2build.h"
#include "iconv.h"
#include "ft_Font.h"
#include "framebuf_public.h"
#include "debug_public.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H
#define TOP_FONT_GLYPH_INTV		0			/*default interval between two glyphs*/
#define TOP_FONT_GLYPH_SPACE	6			/*default length of glyphs space*/

#define _FT_FONT_NULL		((void *)0L)	/* return NULL */
#define _FT_FONT_FAIL		-1L			/* return -1 */
#define _FT_FONT_SUCC		0			/* return 0 */
#define _FT_FONT_YES		1			/* return 1 */
#define _FT_FONT_NO 		0			/* return 0 */

static FT_Library	ft_Font_FTLibrary;
static FT_Face		ft_Font_FtFace;

static int  FT_FONT_STR_LEN	= 1024; /*max length of string to be display*/
static char FT_FONT_FILE_PATH[64] = "/tmp/fzy3jw.ttf";       /*the path of font file*/
//static char FT_FONT_FILE_PATH[64] = "/tmp/yahei.ttf";
static int  FT_FONT_CONV_NUM = 4; /*the number of font type*/

static FT_MEM_BLOCK ft_MEM;

static FT_MEM_BLOCK ft_MEM_SLV;

//#define screen_width gr_w
//#define screen_height gr_h

static int screen_width = 1280;
static int screen_height = 720;


static char ftIconvCharSet[4][16] = {
    "utf-8",
    "gb2312",
    "gbk",
    "utf-16"
};


static int configCharSet_ft_Font(void)
{
    return 0;
}


//设置字符集
static int ft_Font_IconvCharSet(char* pSrc, int lenSrc, unsigned short **ppDst, int *pLenDst, char * pCharSet)
{
    iconv_t varConv;
    size_t 	lenIn, lenOut;
    int     lenDst = 0;
    char    *pOutHead, *pOut;

    *ppDst = NULL;

    /* Convert String */
    lenOut = (lenSrc+1024) * sizeof(short);
    lenIn = lenSrc;
    pOutHead = (char *)malloc(lenOut*sizeof(char));
    if (pOutHead == NULL)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, ">>>>>>>>>>>>>>>memory leak\n");
        return _FT_FONT_FAIL;
    }

    /* Open iconv Lib */
    varConv = iconv_open("UCS-2-INTERNAL", pCharSet);
    if(varConv == (iconv_t)(_FT_FONT_FAIL))
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "Iconv Open %s Fail!\n", pCharSet);
        free(pOutHead);
        pOutHead = NULL;
        return _FT_FONT_FAIL;
    }

    memset(pOutHead, 0, lenOut*sizeof(char));
    pOut = pOutHead;
    while (lenIn > 0)
    {
        if((_FT_FONT_FAIL) == iconv(varConv, (char* *)(&pSrc), &lenIn,	&pOut, &lenOut))
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "Iconv ErrNo: %d!\n", errno);
            free(pOutHead);
            pOutHead = NULL;
            iconv_close(varConv);
            return _FT_FONT_FAIL;
        }
    }

    /* Convert '\0' */
    if((_FT_FONT_FAIL) == iconv(varConv, NULL, NULL, &pOut, &lenOut))
    {
        M_TRACE(DEBUG_TRACE, FT_MODE, "Iconv Str Fail!\n");
        free(pOutHead);
        pOutHead = NULL;
        iconv_close(varConv);
        return _FT_FONT_FAIL;
    }
    /* Set Size */
    lenDst = pOut -pOutHead;
    lenDst >>= 1;    /* Unicode Size */
    *pLenDst = lenDst;
    *ppDst = (unsigned short *)pOutHead;
    iconv_close(varConv);

    if(lenDst > 0)
    {
        //free(pOutHead);
        return _FT_FONT_SUCC;
    }

    free(pOutHead);
    pOutHead = NULL;
    return _FT_FONT_FAIL;
}

int ft_Font_Conv_Unicode_Str(char* pSrc, int lenSrc, unsigned char *pDst, int LenDst)
{
    iconv_t 	varConv;
    size_t 	lenIn, lenOut;
    char    *pOut;
    int i;

    for(i=0; i<1; i++)
    {
        /* Open iconv Lib */
        varConv = iconv_open("GBK", ftIconvCharSet[i]);
        if(varConv == (iconv_t)(_FT_FONT_FAIL))
        {
            M_TRACE(ERROR_TRACE, FT_MODE, "Iconv Open %s Fail!\n", ftIconvCharSet[i]);
            //continue;
            return _FT_FONT_FAIL;
        }

        /* Convert String */
        lenOut = (lenSrc+1024) * sizeof(short);
        lenIn = lenSrc;
        pOut = (char *)pDst;

        while (lenIn > 0)
        {
            if((_FT_FONT_FAIL) == iconv(varConv, (char* *)(&pSrc), &lenIn,	&pOut, &lenOut))
            {
                M_TRACE(DEBUG_TRACE, FT_MODE, "Iconv ErrNo: %d!\n", errno);
                //iconv_close(varConv);
               // return _FT_FONT_FAIL;
                break;
            }
        }

        /* Convert '\0' */
        if((_FT_FONT_FAIL) == iconv(varConv, NULL, NULL, &pOut, &lenOut))
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "Iconv Str Fail!\n");
            iconv_close(varConv);
            return _FT_FONT_FAIL;
            //continue;
        }

        /* Set Size */
        iconv_close(varConv);
        if(lenOut > 0)
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "IConv str success\n");
            return _FT_FONT_SUCC;
        }
    }

    return _FT_FONT_FAIL;
}


//字符转化
static int ft_Font_FontConvStrBufRelease(unsigned short *pBuf)
{
    M_TRACE(DEBUG_TRACE, FT_MODE, "%x\n", pBuf);
    if (pBuf)
        free(pBuf);

    return 0;
}

//static unsigned short *contstr=NULL;
static int ft_Font_FontConvStr(char* pText, unsigned short **ppDst, int *pLenDst)
{
    int i;
    int len;
    if(!pText)
        return _FT_FONT_NO;

    *pLenDst = 0;
    len = strlen(pText);
    if(len > 0)
    {
        if (len > FT_FONT_STR_LEN)
        {
            len = FT_FONT_STR_LEN;
            M_TRACE(DEBUG_TRACE, FT_MODE, "too much string for conv len = %d\n", len);
        }

        for(i=0; i<FT_FONT_CONV_NUM; i++)
        {
            /* 自动转换字符格式 */
            if(_FT_FONT_SUCC == ft_Font_IconvCharSet(pText, len, ppDst, pLenDst, ftIconvCharSet[i]))
            {
                M_TRACE(DEBUG_TRACE, FT_MODE, "Success the Char Set %d\n", i);
                return _FT_FONT_SUCC;
            }
        }
    }

    return _FT_FONT_FAIL;
}


int FontConvTxtStr_2_Unicode(char* pText, unsigned short **ppDst, int *pLenDst)
{
    int i;
    int len;

    if(!pText)
    {
        M_TRACE(WARING_TRACE, FT_MODE, "ptxt is null\r\n");
        return _FT_FONT_NO;
    }

    len = strlen(pText);
    if(len > 0)
    {
        for(i = 0; i < FT_FONT_CONV_NUM; i++)
        {
            /* 自动转换字符格式 */
            if(_FT_FONT_SUCC == ft_Font_IconvCharSet(pText, len, ppDst, pLenDst, ftIconvCharSet[i]))
            {
                M_TRACE(DEBUG_TRACE, FT_MODE, "Success the Char Set %d\n", i);
                return _FT_FONT_SUCC;
            }
        }
    }

    M_TRACE(WARING_TRACE, FT_MODE, "input len is zero\r\n");
    return _FT_FONT_FAIL;
}

static int bInit = 0;
int ft_Font_Init(void)
{
    int ret;

    if (bInit)
        return 0;

    configCharSet_ft_Font();

    ret = FT_Init_FreeType(&ft_Font_FTLibrary);
    if( ret )
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "FT_Init_FreeType: [%08x]\n", ret);
        return _FT_FONT_FAIL;
    }

    ret = FT_New_Face(ft_Font_FTLibrary, FT_FONT_FILE_PATH, 0, &ft_Font_FtFace);
    if( ret )
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "FT_New_Face: [%d]\n", ret);
        return _FT_FONT_FAIL;
    }

    ret = FT_Select_Charmap(ft_Font_FtFace, ft_encoding_unicode);
    if( ret )
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "FT_Select_Charmap: [%d]\n", ret);
        return _FT_FONT_FAIL;
    }

    bInit = 1;

    ft_MEM.cost_mem_width = 0;
    ft_MEM.disp_mem_height = 0;
    ft_MEM.disp_mem_width = 0;
    ft_MEM.pmem = malloc(1280*100*2);

    ft_MEM_SLV.cost_mem_width = 0;
    ft_MEM_SLV.disp_mem_height = 0;
    ft_MEM_SLV.disp_mem_width = 0;
    ft_MEM_SLV.pmem = malloc(1280*100*2);

    return _FT_FONT_SUCC;
}


void ft_Font_Destroy(void)
{
    if (ft_MEM.pmem)
    {
        free(ft_MEM.pmem);
    }

    if (ft_MEM_SLV.pmem)
    {
        free(ft_MEM_SLV.pmem);
    }

    if(ft_Font_FtFace)
    {
        FT_Done_Face(ft_Font_FtFace);
    }

    if(ft_Font_FTLibrary)
    {
        FT_Done_FreeType(ft_Font_FTLibrary);
    }

    return;
}

int ft_Font_Running(void)
{
    return bInit;
}

static int ft_Font_SetCharSize(int fontHeight, int fontWidth)
{
    int 	i, j, jppem;
    /* Font Lib Pixel Size Set */

    if (FT_IS_SCALABLE(ft_Font_FtFace))
    {
        M_TRACE(DEBUG_TRACE, FT_MODE, "FT_IS_SCALABLE(gFtFace)\r\n");
        if (FT_Set_Char_Size(ft_Font_FtFace, fontWidth*64, fontHeight*64, 0, 0))
        {
            M_TRACE(ERROR_TRACE, FT_MODE, "FT_Set_Char_Size Failed!\n");
            return _FT_FONT_FAIL;
        }
    }
    else
    {
        /* Fixed Size Select */
        jppem = ft_Font_FtFace->available_sizes[0].height;
        M_TRACE(DEBUG_TRACE, FT_MODE, "FtFace Fixed Size : %d \n", ft_Font_FtFace->num_fixed_sizes);
        for (i = 0, j = 0; i < ft_Font_FtFace->num_fixed_sizes; i++)
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "FtFace Height : %d \n", ft_Font_FtFace->available_sizes[i].height);

            if (fabs(ft_Font_FtFace->available_sizes[i].height-fontHeight) < abs(jppem-fontHeight))
            {
                jppem = ft_Font_FtFace->available_sizes[i].height;
                j = i;
            }
        }
        M_TRACE(DEBUG_TRACE, FT_MODE, "FtFace Fixed Select PPEM : %d \n", jppem);
        if (FT_Set_Pixel_Sizes(ft_Font_FtFace, ft_Font_FtFace->available_sizes[j].width, jppem))
        {
            M_TRACE(ERROR_TRACE, FT_MODE, "FT_Set_Pixel_Size Failed!\n");
            return _FT_FONT_FAIL;
        }
        fontHeight = jppem;
    }

    return fontHeight;
}

int ft_Font_StringToMem(char *pText, FT_MEM_BLOCK *pMem, unsigned int fontSize, unsigned long int mem_max_len)
{
    FT_GlyphSlot        slot;
    FT_BitmapGlyph      bitmapGlyph;
    FT_Glyph            glyph;
    FT_Bitmap           *pBitmap = NULL;
    int                 glyph_index = 0;
    unsigned short      *pDstString = NULL;
    int                 dstLen = 0;
    int                 i, j, k;

    int mem_width = 0, mem_height = 0;
    int disp_mem_height = 0, width_tag = 0;

    unsigned char       *bitmapBuffer = NULL;
    int buffer_index, buffer_len;

    int ft_transform_active = 0;

    bitmapBuffer = pMem->pmem;
    if(bitmapBuffer == NULL)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "you haven't get memory!\n");
        return -1;
    }

    memset(bitmapBuffer, 0, mem_max_len);

    ft_Font_FontConvStr(pText, &pDstString, &dstLen);
    if(dstLen < 1)
    {
        /*
        if(pDstString)
        {
            free(pDstString);
            ft_Font_FontConvStrBufRelease(pDstString);
        }
        */
        ft_Font_FontConvStrBufRelease(pDstString);
        M_TRACE(ERROR_TRACE, FT_MODE, "no character has been changed success!\n");
        return -2;
    }

    //set the char size
    if( _FT_FONT_FAIL == ft_Font_SetCharSize(fontSize, fontSize))
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "set free type size failure!\r\n");

        ft_Font_FontConvStrBufRelease(pDstString);
        return -3;
    }

    mem_width = dstLen*fontSize;
    mem_height = fontSize*2;
    buffer_len = mem_height*mem_width;

    if(buffer_len >= mem_max_len)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "not a usable mem block %d\n", buffer_len);

        ft_Font_FontConvStrBufRelease(pDstString);
        return -4;
    }
    buffer_index = 0;

    //memset(bitmapBuffer, 0, buffer_len*sizeof(TPPixel64));
    for(i = 0; i < buffer_len; i++)
    {
        bitmapBuffer[i] = 0x00;
    }

    for(i=0; i<dstLen; i++)
    {
        //get the index of current character and load glyph to slot
        glyph_index = FT_Get_Char_Index(ft_Font_FtFace, pDstString[i]);
        if(glyph_index==0)
        {
            //undefined character
            continue;
        }

        if(FT_Load_Glyph(ft_Font_FtFace, glyph_index, FT_LOAD_DEFAULT)!=0)
        {
            //load failure
            continue;
        }

        slot = ft_Font_FtFace->glyph;
        //if the slot format is not ft_glyph_format_bitmap, then render the glyph
        if(slot->format != ft_glyph_format_bitmap)
        {
            if(FT_Render_Glyph(ft_Font_FtFace->glyph, FT_RENDER_MODE_NORMAL) != 0)
            {
                //render failure
                continue;
            }
        }

        if(FT_Get_Glyph(slot, &glyph) != 0)
        {
            //get glyph failure
            continue;
        }

        ft_transform_active++;

        bitmapGlyph = (FT_BitmapGlyph)glyph;
        pBitmap = &(bitmapGlyph->bitmap);

        buffer_index = (fontSize-bitmapGlyph->top)*mem_width + bitmapGlyph->left + width_tag;

        for(j=0; j<pBitmap->rows;j++)
        {
            for(k=0;k<pBitmap->width;k++)
            {
                if(pBitmap->buffer[j*pBitmap->width+k] != 0)
                {
                    bitmapBuffer[buffer_index+j*mem_width+k] = pBitmap->buffer[j*pBitmap->width+k];
                }
            }
        }

        if((pBitmap->rows==0) && (pBitmap->width==0))
        {
            width_tag += TOP_FONT_GLYPH_SPACE;
        }
        else
        {
            width_tag += (bitmapGlyph->left+pBitmap->width);
        }

        if(disp_mem_height < (fontSize-bitmapGlyph->top+pBitmap->rows))
        {
            disp_mem_height = fontSize-bitmapGlyph->top+pBitmap->rows;
        }

        FT_Done_Glyph(glyph);
    }

    if(ft_transform_active == 0)
    {
        //printf("[ft_error], there is not valid transform!\n");
        M_TRACE(ERROR_TRACE, FT_MODE, "there is not valid transform!\n");
        ft_Font_FontConvStrBufRelease(pDstString);
        return -5;
    }

    pMem->cost_mem_width = mem_width;
    pMem->disp_mem_width = width_tag;
    pMem->disp_mem_height = disp_mem_height;

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d][%d][%d]\n",
        pMem->cost_mem_width,
        pMem->disp_mem_height,
        pMem->disp_mem_width);

    //free malloc
    ft_Font_FontConvStrBufRelease(pDstString);
    return 0;
}

int ft_Font_Unicode_ToMem(short *pText, FT_MEM_BLOCK *pMem, unsigned int fontSize, unsigned long int mem_max_len, int uncode_len)
{
    FT_GlyphSlot        slot;
    FT_BitmapGlyph      bitmapGlyph;
    FT_Glyph            glyph;
    FT_Bitmap           *pBitmap = NULL;
    int                 glyph_index = 0;
    unsigned short      *pDstString = NULL;
    int                 dstLen = 0;
    int                 i, j, k;

    int mem_width = 0, mem_height = 0;
    int disp_mem_height = 0, width_tag = 0;

    unsigned char       *bitmapBuffer = NULL;
    int buffer_index, buffer_len;

    int ft_transform_active = 0;

    bitmapBuffer = pMem->pmem;
    if(bitmapBuffer == NULL)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "you haven't get memory!\n");
        return -1;
    }

    memset(bitmapBuffer, 0, mem_max_len);
    pDstString = (unsigned short *) pText;
    dstLen = uncode_len;

    //set the char size
    if( _FT_FONT_FAIL == ft_Font_SetCharSize(fontSize, fontSize))
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "set free type size failure!\r\n");

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -3;
    }

    mem_width = dstLen*fontSize;
    mem_height = fontSize*2;
    buffer_len = mem_height*mem_width;

    if(buffer_len >= mem_max_len)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "not a usable mem block %d\n", buffer_len);

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -4;
    }
    buffer_index = 0;

    //memset(bitmapBuffer, 0, buffer_len*sizeof(TPPixel64));
    for(i = 0; i < buffer_len; i++)
    {
        bitmapBuffer[i] = 0x00;
    }

    for(i=0; i<dstLen; i++)
    {
        //get the index of current character and load glyph to slot
        glyph_index = FT_Get_Char_Index(ft_Font_FtFace, pDstString[i]);
        if(glyph_index==0)
        {
            //undefined character
            continue;
        }

        if(FT_Load_Glyph(ft_Font_FtFace, glyph_index, FT_LOAD_DEFAULT)!=0)
        {
            //load failure
            continue;
        }

        slot = ft_Font_FtFace->glyph;
        //if the slot format is not ft_glyph_format_bitmap, then render the glyph
        if(slot->format != ft_glyph_format_bitmap)
        {
            if(FT_Render_Glyph(ft_Font_FtFace->glyph, FT_RENDER_MODE_NORMAL) != 0)
            {
                //render failure
                continue;
            }
        }

        if(FT_Get_Glyph(slot, &glyph) != 0)
        {
            //get glyph failure
            continue;
        }

        ft_transform_active++;

        bitmapGlyph = (FT_BitmapGlyph)glyph;
        pBitmap = &(bitmapGlyph->bitmap);

        buffer_index = (fontSize-bitmapGlyph->top)*mem_width + bitmapGlyph->left + width_tag;

        for(j=0; j<pBitmap->rows;j++)
        {
            for(k=0;k<pBitmap->width;k++)
            {
                if(pBitmap->buffer[j*pBitmap->width+k] != 0)
                {
                    bitmapBuffer[buffer_index+j*mem_width+k] = pBitmap->buffer[j*pBitmap->width+k];
                }
            }
        }

        if((pBitmap->rows==0) && (pBitmap->width==0))
        {
            width_tag += TOP_FONT_GLYPH_SPACE;
        }
        else
        {
            width_tag += (bitmapGlyph->left+pBitmap->width);
        }

        if(disp_mem_height < (fontSize-bitmapGlyph->top+pBitmap->rows))
        {
            disp_mem_height = fontSize-bitmapGlyph->top+pBitmap->rows;
        }

        FT_Done_Glyph(glyph);
    }

    if(ft_transform_active == 0)
    {
        //printf("[ft_error], there is not valid transform!\n");
        M_TRACE(ERROR_TRACE, FT_MODE, "there is not valid transform!\n");
        //ft_Font_FontConvStrBufRelease(pDstString);
        return -5;
    }

    pMem->cost_mem_width = mem_width;
    pMem->disp_mem_width = width_tag;
    pMem->disp_mem_height = disp_mem_height;

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d][%d][%d]\n",
        pMem->cost_mem_width,
        pMem->disp_mem_height,
        pMem->disp_mem_width);

    //free malloc
    //ft_Font_FontConvStrBufRelease(pDstString);
    return 0;
}

int ft_Font_Unicode_ToMem_with_const_width(short *pText,
        FT_MEM_BLOCK *pMem,
        unsigned int fontSize,
        unsigned long int mem_max_len,
        int uncode_len,
        int width,
        int *pout_len)
{
    FT_GlyphSlot        slot;
    FT_BitmapGlyph      bitmapGlyph;
    FT_Glyph            glyph;
    FT_Bitmap           *pBitmap = NULL;
    int                 glyph_index = 0;
    unsigned short      *pDstString = NULL;
    int                 dstLen = 0;
    int                 i, j, k;

    int mem_width = 0, mem_height = 0;
    int disp_mem_height = 0, width_tag = 0;

    unsigned char       *bitmapBuffer = NULL;
    int buffer_index, buffer_len;

    int ft_transform_active = 0;

    *pout_len = 0;
    bitmapBuffer = pMem->pmem;
    if(bitmapBuffer == NULL)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "you haven't get memory!\n");
        return -1;
    }

    //memset(bitmapBuffer, 0, mem_max_len);
    pDstString = (unsigned short *) pText;
    dstLen = uncode_len;

    //set the char size
    if( _FT_FONT_FAIL == ft_Font_SetCharSize(fontSize, fontSize))
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "set free type size failure!\r\n");

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -3;
    }

    mem_width = width;
    mem_height = fontSize*2;
    buffer_len = mem_height*mem_width;

    M_TRACE(DEBUG_TRACE, FT_MODE, "%d:%d:%d\r\n", mem_width, mem_height, buffer_len);
#if 1
    if(buffer_len >= mem_max_len)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "not a usable mem block %d\n", buffer_len);

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -4;
    }
#endif
    buffer_index = 0;
#if 0
    //memset(bitmapBuffer, 0, buffer_len*sizeof(TPPixel64));
    for(i = 0; i < buffer_len; i++)
    {
        bitmapBuffer[i] = 0x00;
    }
#endif

    for(i=0; i<dstLen; i++)
    {
        *pout_len = i+1;
        //get the index of current character and load glyph to slot
        glyph_index = FT_Get_Char_Index(ft_Font_FtFace, pDstString[i]);
        if(glyph_index==0)
        {
            //undefined character
            continue;
        }

        if(FT_Load_Glyph(ft_Font_FtFace, glyph_index, FT_LOAD_DEFAULT)!=0)
        {
            //load failure
            continue;
        }

        slot = ft_Font_FtFace->glyph;
        //if the slot format is not ft_glyph_format_bitmap, then render the glyph
        if(slot->format != ft_glyph_format_bitmap)
        {
            if(FT_Render_Glyph(ft_Font_FtFace->glyph, FT_RENDER_MODE_NORMAL) != 0)
            {
                //render failure
                continue;
            }
        }

        if(FT_Get_Glyph(slot, &glyph) != 0)
        {
            //get glyph failure
            continue;
        }

        ft_transform_active++;

        bitmapGlyph = (FT_BitmapGlyph)glyph;
        pBitmap = &(bitmapGlyph->bitmap);

        buffer_index = (fontSize-bitmapGlyph->top)*mem_width + bitmapGlyph->left + width_tag;

        for(j=0; j<pBitmap->rows;j++)
        {
            for(k=0;k<pBitmap->width;k++)
            {
                if(pBitmap->buffer[j*pBitmap->width+k] != 0)
                {
                    bitmapBuffer[buffer_index+j*mem_width+k] = pBitmap->buffer[j*pBitmap->width+k];
                }
            }
        }

        if((pBitmap->rows==0) && (pBitmap->width==0))
        {
            width_tag += TOP_FONT_GLYPH_SPACE;
        }
        else
        {
            width_tag += (bitmapGlyph->left+pBitmap->width);
        }

        if(disp_mem_height < (fontSize-bitmapGlyph->top+pBitmap->rows))
        {
            disp_mem_height = fontSize-bitmapGlyph->top+pBitmap->rows;
        }

        FT_Done_Glyph(glyph);

        //if (width_tag >= (width-(fontSize*2/3)) )

        if (width_tag >= (width-fontSize))
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "display width is enough %d %d\r\n", width_tag, width);
            break;
        }

    }

    if(ft_transform_active == 0)
    {
        //printf("[ft_error], there is not valid transform!\n");
        M_TRACE(ERROR_TRACE, FT_MODE, "there is not valid transform!\n");
        //ft_Font_FontConvStrBufRelease(pDstString);
        return -5;
    }

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d][%d][%d]\n",
                mem_width,
                disp_mem_height,
                width_tag);

    {

        int leave_taier = 0;
        int interval_num = 0;
        int insert_space = 1;
        int disp_font_num;

        disp_font_num = ft_transform_active;

        leave_taier = mem_width - width_tag;
        if (leave_taier > 4)
        {
            interval_num = (*pout_len + leave_taier-1)/leave_taier;
            M_TRACE(DEBUG_TRACE, FT_MODE, "modify now %d | %d\r\n", leave_taier, interval_num);
            mem_width = 0;
            mem_height = 0;
            disp_mem_height = 0;
            width_tag = 0;
            //*bitmapBuffer = NULL;

            ft_transform_active = 0;

            *pout_len = 0;
            bitmapBuffer = pMem->pmem;

            memset(bitmapBuffer, 0, mem_max_len);
            pDstString = (unsigned short *) pText;
            dstLen = uncode_len;

            mem_width = width;
            mem_height = fontSize*2;
            buffer_len = mem_height*mem_width;
            buffer_index = 0;

            for(i=0; i<dstLen; i++)
            {
                *pout_len = i+1;
                //get the index of current character and load glyph to slot
                glyph_index = FT_Get_Char_Index(ft_Font_FtFace, pDstString[i]);
                if(glyph_index==0)
                {
                    //undefined character
                    continue;
                }

                if(FT_Load_Glyph(ft_Font_FtFace, glyph_index, FT_LOAD_DEFAULT)!=0)
                {
                    //load failure
                    continue;
                }

                slot = ft_Font_FtFace->glyph;
                //if the slot format is not ft_glyph_format_bitmap, then render the glyph
                if(slot->format != ft_glyph_format_bitmap)
                {
                    if(FT_Render_Glyph(ft_Font_FtFace->glyph, FT_RENDER_MODE_NORMAL) != 0)
                    {
                        //render failure
                        continue;
                    }
                }

                if(FT_Get_Glyph(slot, &glyph) != 0)
                {
                    //get glyph failure
                    continue;
                }

                ft_transform_active++;

                bitmapGlyph = (FT_BitmapGlyph)glyph;
                pBitmap = &(bitmapGlyph->bitmap);

                buffer_index = (fontSize-bitmapGlyph->top)*mem_width + bitmapGlyph->left + width_tag;

                for(j=0; j<pBitmap->rows;j++)
                {
                    for(k=0;k<pBitmap->width;k++)
                    {
                        if(pBitmap->buffer[j*pBitmap->width+k] != 0)
                        {
                            bitmapBuffer[buffer_index+j*mem_width+k] = pBitmap->buffer[j*pBitmap->width+k];
                        }
                    }
                }

                if((pBitmap->rows==0) && (pBitmap->width==0))
                {
                    width_tag += TOP_FONT_GLYPH_SPACE;
                }
                else
                {
                    width_tag += (bitmapGlyph->left+pBitmap->width);
                }

                if(disp_mem_height < (fontSize-bitmapGlyph->top+pBitmap->rows))
                {
                    disp_mem_height = fontSize-bitmapGlyph->top+pBitmap->rows;
                }

                FT_Done_Glyph(glyph);

                if (disp_font_num == ft_transform_active)
                {
                    M_TRACE(DEBUG_TRACE, FT_MODE, "display finish %d|%d\r\n",
                        disp_font_num, ft_transform_active);
                    break;
                }

                if (insert_space*interval_num == i)
                {
                    M_TRACE(DEBUG_TRACE, FT_MODE, "inser space %d\r\n", i);

                    width_tag++;
                    insert_space++;

                }

                //if (width_tag >= (width-(fontSize*2/3)) )

            }

        }

    }


    pMem->cost_mem_width = mem_width;
    pMem->disp_mem_width = width_tag;
    pMem->disp_mem_height = disp_mem_height;

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d][%d][%d]\n",
        pMem->cost_mem_width,
        pMem->disp_mem_height,
        pMem->disp_mem_width);




    return 0;
}

int ft_Font_Unicode_ToMem_with_width(short *pText,
        FT_MEM_BLOCK *pMem,
        unsigned int fontSize,
        unsigned long int mem_max_len,
        int uncode_len,
        int width,
        int *pout_len)
{
    FT_GlyphSlot        slot;
    FT_BitmapGlyph      bitmapGlyph;
    FT_Glyph            glyph;
    FT_Bitmap           *pBitmap = NULL;
    int                 glyph_index = 0;
    unsigned short      *pDstString = NULL;
    int                 dstLen = 0;
    int                 i, j, k;

    int mem_width = 0, mem_height = 0;
    int disp_mem_height = 0, width_tag = 0;

    unsigned char       *bitmapBuffer = NULL;
    int buffer_index, buffer_len;

    int ft_transform_active = 0;

    *pout_len = 0;
    bitmapBuffer = pMem->pmem;
    if(bitmapBuffer == NULL)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "you haven't get memory!\n");
        return -1;
    }

    //memset(bitmapBuffer, 0, mem_max_len);
    pDstString = (unsigned short *) pText;
    dstLen = uncode_len;

    //set the char size
    if( _FT_FONT_FAIL == ft_Font_SetCharSize(fontSize, fontSize))
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "set free type size failure!\r\n");

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -3;
    }

    mem_width = width;
    mem_height = fontSize*2;
    buffer_len = mem_height*mem_width;

    M_TRACE(DEBUG_TRACE, FT_MODE, "%d:%d:%d\r\n", mem_width, mem_height, buffer_len);
#if 1
    if(buffer_len >= mem_max_len)
    {
        M_TRACE(ERROR_TRACE, FT_MODE, "not a usable mem block %d\n", buffer_len);

        //ft_Font_FontConvStrBufRelease(pDstString);
        return -4;
    }
#endif
    buffer_index = 0;
#if 0
    //memset(bitmapBuffer, 0, buffer_len*sizeof(TPPixel64));
    for(i = 0; i < buffer_len; i++)
    {
        bitmapBuffer[i] = 0x00;
    }
#endif

    for(i=0; i<dstLen; i++)
    {
        *pout_len = i+1;
        //get the index of current character and load glyph to slot
        glyph_index = FT_Get_Char_Index(ft_Font_FtFace, pDstString[i]);
        if(glyph_index==0)
        {
            //undefined character
            continue;
        }

        if(FT_Load_Glyph(ft_Font_FtFace, glyph_index, FT_LOAD_DEFAULT)!=0)
        {
            //load failure
            continue;
        }

        slot = ft_Font_FtFace->glyph;
        //if the slot format is not ft_glyph_format_bitmap, then render the glyph
        if(slot->format != ft_glyph_format_bitmap)
        {
            if(FT_Render_Glyph(ft_Font_FtFace->glyph, FT_RENDER_MODE_NORMAL) != 0)
            {
                //render failure
                continue;
            }
        }

        if(FT_Get_Glyph(slot, &glyph) != 0)
        {
            //get glyph failure
            continue;
        }

        ft_transform_active++;

        bitmapGlyph = (FT_BitmapGlyph)glyph;
        pBitmap = &(bitmapGlyph->bitmap);

        buffer_index = (fontSize-bitmapGlyph->top)*mem_width + bitmapGlyph->left + width_tag;

        for(j=0; j<pBitmap->rows;j++)
        {
            for(k=0;k<pBitmap->width;k++)
            {
                if(pBitmap->buffer[j*pBitmap->width+k] != 0)
                {
                    bitmapBuffer[buffer_index+j*mem_width+k] = pBitmap->buffer[j*pBitmap->width+k];
                }
            }
        }

        if((pBitmap->rows==0) && (pBitmap->width==0))
        {
            width_tag += TOP_FONT_GLYPH_SPACE;
        }
        else
        {
            width_tag += (bitmapGlyph->left+pBitmap->width);
        }

        if(disp_mem_height < (fontSize-bitmapGlyph->top+pBitmap->rows))
        {
            disp_mem_height = fontSize-bitmapGlyph->top+pBitmap->rows;
        }

        FT_Done_Glyph(glyph);

        if (width_tag >= (width-(fontSize*2/3)) )
        {
            M_TRACE(DEBUG_TRACE, FT_MODE, "display width is enough %d %d\r\n", width_tag, width);
            break;
        }
    }

    if(ft_transform_active == 0)
    {
        //printf("[ft_error], there is not valid transform!\n");
        M_TRACE(ERROR_TRACE, FT_MODE, "there is not valid transform!\n");
        //ft_Font_FontConvStrBufRelease(pDstString);
        return -5;
    }

    pMem->cost_mem_width = mem_width;
    pMem->disp_mem_width = width_tag;
    pMem->disp_mem_height = disp_mem_height;

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d][%d][%d]\n",
        pMem->cost_mem_width,
        pMem->disp_mem_height,
        pMem->disp_mem_width);

    //free malloc
    //ft_Font_FontConvStrBufRelease(pDstString);
    return 0;
}


void ft_Font_Draw_2_Canvas(FT_MEM_BLOCK *pMem,
            Pixel64 *pCanvas,
            Pixel64 FT,
            GrPos Pos)
{
    int i, j;

    int disp_length ;
    int disp_height ;
    //	TPPixel64 *pSrcTPPixel;
    Pixel64 *pDstTPPixel;

    int srcAlpha = 0, dstAlpha = 0;

    if ((pMem == NULL) || (pCanvas == NULL))
        return ;

    if(pMem->disp_mem_width > screen_width)
    {
        disp_length = screen_width;
    }
    else
    {
        disp_length = pMem->disp_mem_width;
    }

    if (pMem->disp_mem_height > screen_height)
    {
        disp_height = screen_height;
    }
    else
    {
        disp_height = pMem->disp_mem_height;
    }

    pCanvas += (Pos.x + Pos.y *screen_width);

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d, %d]\n", disp_height, disp_length);
    for(i = 0; i < disp_height; i++)
    {
        for(j = 0; j < disp_length; j++)
        {
            //pSurface[i*1280+j].value32 = special_text_mem_block.pmem[i*special_text_mem_block.cost_mem_width+j].value32;
            FT.RGB.a = pMem->pmem[i*pMem->cost_mem_width+j];

            pDstTPPixel = pCanvas+i*screen_width+j;

            if(FT.RGB.a == 0xFF)
            {
                pDstTPPixel->value32 = FT.value32;
            }
            else if(FT.RGB.a > 0)
            {
                srcAlpha = FT.RGB.a;
                dstAlpha = 255 - FT.RGB.a;

                pDstTPPixel->RGB.r = (pDstTPPixel->RGB.r*dstAlpha + FT.RGB.r*srcAlpha)>>8;
                pDstTPPixel->RGB.g = (pDstTPPixel->RGB.g*dstAlpha + FT.RGB.g*srcAlpha)>>8;
                pDstTPPixel->RGB.b = (pDstTPPixel->RGB.b*dstAlpha + FT.RGB.b*srcAlpha)>>8;
            }
        }
    }

}

void ft_Font_Draw_2_Canvas_with_region(FT_MEM_BLOCK *pMem,
            Pixel64 *pCanvas,
            Pixel64 FT,
            GrPos Pos,
            GrRegion Region)
{
    int i, j;

    int disp_length ;
    int disp_height ;
    //	TPPixel64 *pSrcTPPixel;
    Pixel64 *pDstTPPixel;

    int srcAlpha = 0, dstAlpha = 0;

    if ((pMem == NULL) || (pCanvas == NULL))
        return ;

    if(pMem->disp_mem_width > Region.w)
    {
        disp_length = Region.w;
    }
    else
    {
        disp_length = pMem->disp_mem_width;
    }

    if (pMem->disp_mem_height > Region.h)
    {
        disp_height = Region.h;
    }
    else
    {
        disp_height = pMem->disp_mem_height;
    }

    pCanvas += (Pos.x + Pos.y *screen_width);

    M_TRACE(DEBUG_TRACE, FT_MODE, "[%d, %d]\n", disp_height, disp_length);
    for(i = 0; i < disp_height; i++)
    {
        for(j = 0; j < disp_length; j++)
        {
            //pSurface[i*1280+j].value32 = special_text_mem_block.pmem[i*special_text_mem_block.cost_mem_width+j].value32;
            FT.RGB.a = pMem->pmem[i*pMem->cost_mem_width+j];

            pDstTPPixel = pCanvas+i*screen_width+j;

            if(FT.RGB.a == 0xFF)
            {
                pDstTPPixel->value32 = FT.value32;
            }
            else if(FT.RGB.a > 0)
            {
                srcAlpha = FT.RGB.a;
                dstAlpha = 255 - FT.RGB.a;

                pDstTPPixel->RGB.r = (pDstTPPixel->RGB.r*dstAlpha + FT.RGB.r*srcAlpha)>>8;
                pDstTPPixel->RGB.g = (pDstTPPixel->RGB.g*dstAlpha + FT.RGB.g*srcAlpha)>>8;
                pDstTPPixel->RGB.b = (pDstTPPixel->RGB.b*dstAlpha + FT.RGB.b*srcAlpha)>>8;
            }
        }
    }
}


FT_MEM_BLOCK * ft_Font_Get_Default_pmem(void)
{
    return &ft_MEM;
}

int ft_Font_Str2Disp(char *str, Pixel64 FT, GrPos pos, int size)
{
    if (ft_Font_StringToMem(str, &ft_MEM, size, (1280*100*2))== 0)
    {

        ft_Font_Draw_2_Canvas(&ft_MEM,
                (Pixel64 *)getFramebuf(),
                FT,
                pos);

        M_TRACE(DEBUG_TRACE, FT_MODE, "width = %d, height = %d\n", ft_MEM.disp_mem_width, ft_MEM.disp_mem_height);
        return 1;
    }

    return 0;
}

int ft_Font_Str2Disp_return_region(char *str, Pixel64 FT, GrPos pos, int size, GrRegion *pRegion)
{
    if (ft_Font_StringToMem(str, &ft_MEM, size, (1280*100*2))== 0)
    {

        ft_Font_Draw_2_Canvas(&ft_MEM,
                (Pixel64 *)getFramebuf(),
                FT,
                pos);

        M_TRACE(DEBUG_TRACE, FT_MODE, "width = %d, height = %d\n", ft_MEM.disp_mem_width, ft_MEM.disp_mem_height);
        pRegion->w = ft_MEM.disp_mem_width;
        pRegion->h = ft_MEM.disp_mem_height;

        return 1;
    }

    return 0;
}


int ft_Font_Str2Disp_with_region(char *str, Pixel64 FT, GrPos pos, int size, GrRegion region)
{
    if (ft_Font_StringToMem(str, &ft_MEM, size, (1280*100*2))== 0)
    {

        ft_Font_Draw_2_Canvas_with_region(&ft_MEM,
                (Pixel64 *)getFramebuf(),
                FT,
                pos,
                region);

        //refresh_main_logic_2_device(pos, region);

        return 1;
    }

    return 0;
}


int ft_Font_Str2Canvas(char *str, Pixel64 FT, Pixel64 *pCanvas, GrPos Pos, int size)
{
    if (ft_Font_StringToMem(str, &ft_MEM, size, (1280*100*2)) == 0)
    {
        ft_Font_Draw_2_Canvas(&ft_MEM, pCanvas, FT, Pos);

        return 1;
    }

    return 0;
}

int ft_Font_Str2Canvas_return_region(char *str, Pixel64 FT, Pixel64 *pCanvas, GrPos Pos, int size, GrRegion *pRegion)
{
    if (ft_Font_StringToMem(str, &ft_MEM, size, (1280*100*2)) == 0)
    {
        ft_Font_Draw_2_Canvas(&ft_MEM, pCanvas, FT, Pos);

        pRegion->w = ft_MEM.disp_mem_width;
        pRegion->h = ft_MEM.disp_mem_height;

        return 1;
    }

    return 0;
}


#define UTF8_ONE_START      (0x0001)
#define UTF8_ONE_END        (0x007F)
#define UTF8_TWO_START      (0x0080)
#define UTF8_TWO_END        (0x07FF)
#define UTF8_THREE_START    (0x0800)
#define UTF8_THREE_END      (0xFFFF)

void UTF16ToUTF8(unsigned short * pUTF16Start,
        unsigned short * pUTF16End,
        unsigned char * pUTF8Start,
        unsigned char * pUTF8End)
{
    unsigned short *pTempUTF16 = pUTF16Start;
	unsigned char  *pTempUTF8 = pUTF8Start;

    int length;
    length = pUTF16End-pTempUTF16;
    //fprintf(stderr, "%d\r\n", length);

    while (pTempUTF16 < pUTF16End)
    {
        //fprintf(stderr, "%04x:", *pTempUTF16);
        if (*pTempUTF16 <= UTF8_ONE_END
            && pTempUTF8 + 1 < pUTF8End)
        {
            //0000 - 007F  0xxxxxxx
            *pTempUTF8++ = (unsigned char)*pTempUTF16;

        }
        else if(*pTempUTF16 >= UTF8_TWO_START
            && *pTempUTF16 <= UTF8_TWO_END
            && pTempUTF8 + 2 < pUTF8End)
        {
            //0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else if(*pTempUTF16 >= UTF8_THREE_START
            && *pTempUTF16 <= UTF8_THREE_END
            && pTempUTF8 + 3 < pUTF8End)
        {
            //0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
            *pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else
        {

            //break;
        }

        pTempUTF16++;
    }

    *pTempUTF8 = 0;
}


