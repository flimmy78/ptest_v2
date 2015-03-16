#ifndef _GPIO_PUBLIC_H_
#define _GPIO_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

int read_gpio_status(unsigned int gpio_num, int *pvalue);
int write_gpio_status(unsigned int gpio_num, int value);


#ifdef __cplusplus
}
#endif

#endif

