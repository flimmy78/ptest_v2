#include "sys_include.h"
#include "gpio_public.h"

static int bSharp = 1;
static int gpio40_value = 0;
static int gpio41_value = 0;
static int gpio44_value = 0;
static struct timeval record_tm;

int init_gpio_borad(void)
{

    //read_gpio_status(44, &gpio44_value);
    write_gpio_status(44, &gpio44_value);

    write_gpio_status(40, gpio40_value);
    write_gpio_status(41, gpio41_value);

    gettimeofday(&(record_tm), NULL);

    return 0;
}


int gpio_sharp_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;

    gettimeofday(&cur_tm, NULL);

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(record_tm.tv_sec*1000+(record_tm.tv_usec)/1000));
    if (distms >= 5*1000)
    {
        if (bSharp)
        {
            write_gpio_status(40, gpio40_value);
            write_gpio_status(41, gpio41_value);

            //M_TRACE(DEBUG_TRACE, SYS_MODE, "gpio40 = %d, gpio41 = %d\n", gpio40_value, gpio41_value);
            if (gpio40_value == 0)
            {
                gpio40_value = 1;
            }
            else
            {
                gpio40_value = 0;
            }

            if (gpio41_value == 0)
            {
                gpio41_value = 1;
            }
            else
            {
                gpio41_value = 0;
            }

        }

        record_tm = cur_tm;
        return 1;
    }

    ret = read_gpio_status(44, &gpio44_value);
    if (ret)
    {
        if (gpio44_value == 0)
        {
            bSharp = 1;
        }
        else
        {
            bSharp = 0;
        }
    }

    return 0;
}


