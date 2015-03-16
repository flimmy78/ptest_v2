
#ifndef _MY_TYPE_DEFINE_H_
#define _MY_TYPE_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    struct {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    } RGB;
    struct {
        unsigned char y;
        unsigned char u;
        unsigned char v;
        unsigned char a;
    } YUV;

    unsigned int 	value32;
}Pixel64;

typedef struct{
    int           x;
    int           y;
    int           w;
    int           h;
}GrRegion;

typedef struct {
	int 			x;
	int 			y;
}GrPos;

typedef struct {
	int				hour;
	int				minute;
	int				second;
}GrTime;

typedef struct {
	int 		year;
	int 		month;
	int 		date;
}GrDate;

typedef enum {

    file_udf,

    dir_en,
    vid_en,
    aud_en,
    pic_en,
    upg_en,

    txt_en,

    max_file_type

}TP_FileType;

#define Xos_C2AU(varArray)  (unsigned int)(sizeof(varArray)/sizeof((varArray)[0]))

#ifdef __cplusplus
}
#endif

#endif

