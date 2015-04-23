#include "sys_include.h"
#include "gpio_public.h"

static int bSharp = 1;
static int gpio_reset = 0;
static int gpio_sata = 0;
static int gpio_sys = 0;
static struct timeval record_tm;
static struct timeval gpio_force_exit_tm;
static int bgpio_quit = 0;

int init_gpio_borad(void)
{

    //read_gpio_status(59, &gpio_reset);
    write_gpio_status(59, &gpio_reset);

    write_gpio_status(58, gpio_sata);
    write_gpio_status(57, gpio_sys);

    gettimeofday(&(record_tm), NULL);
    gettimeofday(&(gpio_force_exit_tm), NULL);

    return 0;
}

int get_force_exit_gpio(void)
{
    return bgpio_quit;
}

int gpio_sharp_filter(void)
{
    static struct timeval cur_tm;
    unsigned int distms = 0;
    int ret;

    gettimeofday(&cur_tm, NULL);

    if (bgpio_quit == 0)
    {
        distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(gpio_force_exit_tm.tv_sec*1000+(gpio_force_exit_tm.tv_usec)/1000));
        if (distms >= 90*1000)
        {
            bgpio_quit = 1;
        }
    }

    distms = ((cur_tm.tv_sec*1000+(cur_tm.tv_usec)/1000)-(record_tm.tv_sec*1000+(record_tm.tv_usec)/1000));
    if (distms >= 200)
    {
        if (bSharp)
        {
            write_gpio_status(58, gpio_sata);
            write_gpio_status(57, gpio_sys);

            //M_TRACE(DEBUG_TRACE, SYS_MODE, "gpio_sata = %d, gpio_sys = %d\n", gpio_sata, gpio_sys);
            if (gpio_sata == 0)
            {
                gpio_sata = 1;
            }
            else
            {
                gpio_sata = 0;
            }

            if (gpio_sys == 0)
            {
                gpio_sys = 1;
            }
            else
            {
                gpio_sys = 0;
            }

        }

        record_tm = cur_tm;
        return 1;
    }

    ret = read_gpio_status(59, &gpio_reset);
    //if (ret)
    {
        if (gpio_reset == 0)
        {
            set_test_status(gpio_test);
            bSharp = 0;
        }
        else
        {
            bSharp = 1;
        }
    }

    return 0;
}


