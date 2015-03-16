#include "sys_include.h"
#include "debugPublic.h"
#include "imgPublic.h"

static Pixel64 * gdecRawCanvas = NULL;

int saveRawFile(char *fileName, Pixel64 *pCanvas, int height, int width)
{
    FILE *wfp = NULL;

    wfp = fopen(fileName, "wb");
    if (wfp == NULL)
    {
        return 0;
    }

    fwrite(&height, 1, 4, wfp);
    fwrite(&width, 1, 4, wfp);
    fwrite(pCanvas, height*width, 4, wfp);
    fclose(wfp);
    return 1;
}

//Ω‚¬ÎRAWÕº∆¨
int decRawFile(char *fileName, Pixel64 **ppCanvas, int *height, int *width)
{
	FILE *rfp = NULL;
    char *buf = NULL;
    int length;

	if (gdecRawCanvas != NULL)
	{
		free(gdecRawCanvas);
		gdecRawCanvas = NULL;
	}

	rfp = fopen(fileName, "rb");
    if (NULL == rfp)
    {
        M_TRACE(ERROR_TRACE, IMG_MODE, "dec RAW file failure [can not open the %s file]\r\n", fileName);
        return -1;
    }

    fread(height, 1, 4, rfp);
    fread(width, 1, 4, rfp);

    length = (*height) * (*width) * 4;
    buf = malloc(length);
    if (buf == NULL)
    {
        fclose(rfp);
        M_TRACE(ERROR_TRACE, IMG_MODE, "malloc failure\r\n");
        return -3;
    }

	gdecRawCanvas = (Pixel64 *) buf;

    fread(buf, length, 1, rfp);

    *ppCanvas = gdecRawCanvas;

    fclose(rfp);

	return 1;
}

int releaseDecRawFileRes(void)
{
	int ret = 0;

	if (gdecRawCanvas != NULL)
	{
		free(gdecRawCanvas);
		gdecRawCanvas = NULL;
		ret = 1;
	}

	return ret;
}

