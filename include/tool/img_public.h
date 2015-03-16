#include "mytype.h"

#ifndef _IMG_PUBLIC_H_
#define _IMG_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif


#define GR_SURFACE_OFFSET(pSurface, offsetX, offestY, scanPitch) \
			((pSurface) + (scanPitch)*(offestY) + (offsetX) )


int decJpgFile(char *fileName, Pixel64 **ppData, int *height, int *width);
int releaseDecJpgFileRes(void);

int decPngFile(char * filename, Pixel64 **ppCanvas, int * pWidth, int * pHeight);
int releaseDecPngFileRes(void);

int saveRawFile(char *fileName, Pixel64 *pCanvas, int height, int width);
int decRawFile(char *fileName, Pixel64 **ppCanvas, int *height, int *width);
int releaseDecRawFileRes(void);

#ifdef __cplusplus
}
#endif

#endif

