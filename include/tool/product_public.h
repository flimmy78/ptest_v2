
#ifndef _PRODUCT_PUBLIC_H_
#define _PRODUCT_PUBLIC_H_

int set_test_network(int number);
int confirm_tfcard_running(void);
int confirm_msata_running(void);


int set_pcba_mac(char *mac, int length);
int get_pcba_mac(char *mac, int length);
int set_pcba_sn(char *sn, int length);
int get_pcba_sn(char *sn, int length);

#endif

