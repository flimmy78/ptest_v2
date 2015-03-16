
#ifndef _DISPLAY_PUBLIC_H_
#define _DISPLAY_PUBLIC_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FMT_1080P_60 = 0,     /**<1080p 60 Hz*/
    FMT_1080P_50,         /**<1080p 50 Hz*/
    FMT_1080P_30,         /**<1080p 30 Hz*/
    FMT_1080P_25,         /**<1080p 25 Hz*/
    FMT_1080P_24,         /**<1080p 24 Hz*/

    FMT_1080i_60,         /**<1080i 60 Hz*/
    FMT_1080i_50,         /**<1080i 50 Hz*/

    FMT_720P_60,          /**<720p 60 Hz*/
    FMT_720P_50,          /**<720p 50 Hz */

    FMT_576P_50,          /**<576p 50 Hz*/
    FMT_480P_60,          /**<480p 60 Hz*/

    MAX_FMT_EN_
}DISP_OUTPUT_TYPE;


int init_display_output_device(DISP_OUTPUT_TYPE type);

int close_display_output_device(void);

#ifdef __cplusplus
}
#endif

#endif

