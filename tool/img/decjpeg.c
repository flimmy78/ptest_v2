
#include "sys_include.h"
#include <setjmp.h>

//#include "debugPublic.h"
#include "img_public.h"
#include "jpeglib.h"

static Pixel64 * gdecJpgCanvas = NULL;

typedef struct _jpg_ehandler {
	struct jpeg_error_mgr  pub;

	jmp_buf                 setjmp_buffer;
}St_JpgEHdl;


static void jpgErrHandler(j_common_ptr cinfo)
{
    St_JpgEHdl *  err;

    err= (St_JpgEHdl * ) cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(err->setjmp_buffer, 1);
}


#define 	XOS_EX8_OP(varIter, varLen, varOp)\
do{\
	varIter = varLen >> 3;\
	while(varIter--){	\
		varOp; 			varOp; 			\
		varOp; 			varOp; 			\
		varOp; 			varOp; 			\
		varOp; 			varOp; 			\
	};\
	varIter = varLen & 0X04;\
	if(varIter){	\
		varOp;			varOp;			\
		varOp;			varOp;			\
	};\
	varIter = varLen & 0X03;\
	while(varIter--){	\
		varOp;			\
	};\
}while(0)


#define 	gImgAU_Proc 	1
static char  	gImgAU_Pixel[gImgAU_Proc][5120];
static JSAMPROW	gImgAU_Row[gImgAU_Proc];

//½âÂëjpegÍ¼Æ¬
int decJpgFile(char *fileName, Pixel64 **ppCanvas, int *height, int *width)
{
	int ret = 0;
	FILE *rfp = NULL;
    St_JpgEHdl jerr;

    struct jpeg_decompress_struct cinfo;
    int i, row_stride, comps;
    JSAMPARRAY  buffer;
    int pitch;
    int *pColor, *pPixel;
    char *buf;
    int j;

    Pixel64 *pCanvas;

	if (gdecJpgCanvas != NULL)
	{
		free(gdecJpgCanvas);
		gdecJpgCanvas = NULL;
	}

	rfp = fopen(fileName, "rb");
    if (NULL == rfp)
    {
        M_TRACE(ERROR_TRACE, IMG_MODE, "dec Jpg file failure [can not open the %s file]\r\n", fileName);
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpgErrHandler;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(rfp);
        M_TRACE(ERROR_TRACE, IMG_MODE, "setjmp failure\r\n");

        return -2;
    }

    M_TRACE(DEBUG_TRACE, IMG_MODE, "start the decompress\r\n");
    /* Start Decompress */
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, rfp);

    (void) jpeg_read_header(&cinfo, TRUE);
    M_TRACE(DEBUG_TRACE, IMG_MODE, "the width = %d, the height = %d\r\n", cinfo.image_width, cinfo.image_height);

    buf = malloc(cinfo.image_width * cinfo.image_height * 4);
    if (buf == NULL)
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(rfp);
        M_TRACE(ERROR_TRACE, IMG_MODE, "malloc failure\r\n");
        return -3;
    }

	gdecJpgCanvas = (Pixel64 *) buf;

    pitch = cinfo.image_width;

    *height = cinfo.image_height;
    *width = cinfo.image_width;

    (void) jpeg_start_decompress(&cinfo);

    M_TRACE(DEBUG_TRACE, IMG_MODE, "out width = %d, height = %d\n", cinfo.output_width, cinfo.output_height);

    comps = cinfo.output_components;
    row_stride = cinfo.output_width * comps;
#if 1
    /* Malloc the buffer for RowStride */
    for(i=0;i<gImgAU_Proc;i++)
    {
        gImgAU_Row[i] = (JSAMPROW)(gImgAU_Pixel[i]);
        for ( j = 0; j  < 5120; j++)
        {
            gImgAU_Pixel[i][j] = 0;
        }
    }
    buffer = (JSAMPARRAY)gImgAU_Row;
#endif

    /* Get the safe width */
    row_stride = (cinfo.output_width < 5120)?(cinfo.output_width):(5120);
    M_TRACE(DEBUG_TRACE, IMG_MODE, "row_stride = %d\n", row_stride);

    pPixel = (int *)buf;
    pCanvas = gdecJpgCanvas;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        i = jpeg_read_scanlines(&cinfo, buffer, gImgAU_Proc);
        buf = &(gImgAU_Pixel[0][0]);
        for (j = 0; j < row_stride; j++)
        {
            pCanvas[j].RGB.a = 255;
            pCanvas[j].RGB.r = buf[j*3];
            pCanvas[j].RGB.g = buf[j*3+1];
            pCanvas[j].RGB.b = buf[j*3+2];
        }
        pCanvas += row_stride;

        #if 0
        /* transform */
        pColor=(int *)(gImgAU_Row[0]);
        XOS_EX8_OP(i, row_stride,
            {
                *pPixel++ = *pColor++;
            });
        pPixel += pitch-row_stride;
        #endif

    }
    M_TRACE(DEBUG_TRACE, IMG_MODE, "finish the decompress\n");

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    *ppCanvas = gdecJpgCanvas;

    fclose(rfp);

	return ret;
}

int releaseDecJpgFileRes(void)
{
	int ret = 0;

	if (gdecJpgCanvas != NULL)
	{
		free(gdecJpgCanvas);
		gdecJpgCanvas = NULL;
		ret = 1;
	}

	return ret;
}


