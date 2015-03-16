#include "sys_include.h"

#include "hifb.h"
#include "hi_unf_disp.h"
#include "hi_adp_data.h"
#include "hi_adp_mpi.h"

#include "display_public.h"

int init_display_output_device(DISP_OUTPUT_TYPE type)
{
    int ret = 0;

    HI_SYS_Init();

    HIADP_MCE_Exit();

    switch (type)
    {
    case FMT_1080P_60:    /**<1080p 60 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_60);
    break;
    case FMT_1080P_50:    /**<1080p 50 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_50);
    break;
    case FMT_1080P_30:    /**<1080p 30 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_30);
    break;
    case FMT_1080P_25:    /**<1080p 25 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_25);
    break;
    case FMT_1080P_24:    /**<1080p 24 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_24);
    break;
    case FMT_1080i_60:    /**<1080i 60 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_60);
    break;
    case FMT_1080i_50:    /**<1080i 50 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_50);
    break;

    case FMT_720P_60:     /**<720p 60 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_60);
    break;
    case FMT_720P_50:     /**<720p 50 Hz */
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    break;

    case FMT_576P_50:     /**<576p 50 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_576P_50);
    break;
    case FMT_480P_60:     /**<480p 60 Hz*/
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_480P_60);
    break;
    default:
        ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    break;
    }

    if (ret != HI_SUCCESS)
    {
        return 0;
    }

    return 1;
}

int close_display_output_device(void)
{
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();

    return 1;
}

