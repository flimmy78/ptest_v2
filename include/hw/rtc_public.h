#ifndef _RTC_PUBLIC_H_
#define _RTC_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

int save_time_into_rtc(void);
int sync_time_from_rtc(void);

#ifdef __cplusplus
}
#endif

#endif

