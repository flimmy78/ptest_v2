
#include "sys_include.h"
#include <setjmp.h>

#include <img_public.h>

#include "png.h"

static Pixel64 * gdecPngCanvas = NULL;

#define 	GrOSD_Width 	1280
#define 	GrOSD_Height	720

#define 	GrOSD_PitchP	1280
#define 	GrOSD_PitchB	5120

int decPngFile(char * filename, Pixel64 **ppCanvas, int * pWidth, int * pHeight)
{
	png_structp 	png_ptr;
	png_infop 		info_ptr;

	png_uint_32 	width, height;
	int 			flag, i;
	FILE 		*	fp;

	png_bytep 		row_stride[GrOSD_Height];

	flag = -1;
	*pWidth = 0;
	*pHeight= 0;

    if (gdecPngCanvas != NULL)
    {
        free(gdecPngCanvas);
        gdecPngCanvas = NULL;
    }

	M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Accer Decode Start:[%s]\n", filename);
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL){
		M_TRACE(WARING_TRACE, IMG_MODE, "PNG Create Read Struct Fail:[%s]\n", filename);
		return (flag);
	}

	/* Open File */
	if ((fp = fopen(filename, "rb")) == NULL){
		M_TRACE(WARING_TRACE, IMG_MODE, "PNG Create Read File Fail:[%s]\n", filename);
		return (flag);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL){
		M_TRACE(WARING_TRACE, IMG_MODE, "PNG Create Info Struct:[%s]\n", filename);
		goto __PNG_EXIT;
	}

	if (setjmp(png_jmpbuf(png_ptr))){
		M_TRACE(WARING_TRACE, IMG_MODE, "PNG SetJmp Struct:[%s]\n", filename);
		goto __PNG_EXIT;
	}

    /* Setup PNG */
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_set_bgr(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Setup Read Info:[%s]\n", filename);

    width 	=(int) info_ptr->width;
    height 	=(int) info_ptr->height;


    gdecPngCanvas = (Pixel64 *)malloc(width*height*4);
    if (gdecPngCanvas == NULL)
    {
        //fclose(fp);
        goto __PNG_EXIT;
    }
	M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Width:%d Height:%d Row:%d\n", width, height, png_get_rowbytes(png_ptr, info_ptr));
#if 0
	if	((width >= pRegion->w) || (height >= pRegion->h)){
		TOP_TRACE(WARING_TRACE, TOOL_MODE, "PNG OutOfRange: Width:%d Height:%d \n", width, height);
		goto __PNG_EXIT;
	}
#endif
    for (i = 0; i < height; i++){
        row_stride[i] = (png_bytep)(GR_SURFACE_OFFSET(gdecPngCanvas, 0, i, width));
    }

    png_read_image(png_ptr, row_stride);
    M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Read Image:[%s]\n", filename);
    png_read_end(png_ptr, info_ptr);
    M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Read End:[%s]\n", filename);

    flag = 0;
    *pWidth = width;
    *pHeight= height;
    M_TRACE(DEBUG_TRACE, IMG_MODE, "PNG Accer Decode End:[%s]\n", filename);
    *ppCanvas = gdecPngCanvas;

__PNG_EXIT:
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	return (0);
}

int releaseDecPngFileRes(void)
{
	int ret = 0;

	if (gdecPngCanvas != NULL)
	{
		free(gdecPngCanvas);
		gdecPngCanvas = NULL;
		ret = 1;
	}

	return ret;
}


