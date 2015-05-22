
#ifndef _SYS_INCLUDE_H_
#define _SYS_INCLUDE_H_

#ifdef __cplusplus
extern "C" {
#endif
/* system include file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <math.h>
#include <time.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/statvfs.h>

#include <signal.h>

#include <pthread.h>
#include <semaphore.h>
#include <sys/poll.h>

#include <assert.h>

/* create by project */
#include <mytype.h>


#include "debug_public.h"

//#define SYSTEM_TEST_SUPPORT 1

#define MAX_DISP_CHAR 1024

typedef enum {

    WELLCOM_STR,
    IP_ADDRESS_SET,

    IP_ADDRESS_SET_OK,
    IP_ADDRESS_SET_FAILED,

    NETWORK_OK,
    NETWORK_FAILED,

    SERIES_OK,
    SERIES_FAILED,

    MSATA_OK,
    MSATA_FAILED,

	register_OK,
	register_FAILED,

	gps_OK,
	gps_FAILED,

	voltage_OK,
	voltage_FAILED_L,
	voltage_FAILED_H,

    TFCARD_OK,
    TFCARD_FAILED,

    VIDEO_CONFIRM,

    INFO_INPUT,

    CONFIRM_SERIES,

    USB_OK,
    USB_FAILED,

    SERIES2_FAILED,
    SERIES2_OK,

    GPIO_PUTDOWN,

    GPIO_TEST,

    MUTE_MIC_TEST,
    MAX_DISP_CHARACTER
}DISP_CHARACTER;

#define usb_ok 0x01
#define msata_ok 0x02
#define sdcard_ok 0x04
#define temperature_failure 0x08
#define rtc_failure 0x10
#define audio_ok 0x20
#define gpio_test 0x40

void set_test_status(unsigned int flag);
void save_test_status(void);
void set_audio_failure(void);

#ifdef __cplusplus
}
#endif


#endif

