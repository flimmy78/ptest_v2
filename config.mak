#---------------Commmand Macro Definition-----------------:

CC		= arm-hisiv200-linux-gcc
CXX		= arm-hisiv200-linux-g++
LD		= arm-hisiv200-linux-ld
AR		= arm-hisiv200-linux-ar
LDA		= arm-hisiv200-linux-g++
STRIP = arm-hisiv200-linux-strip

RM		= rm -fr
CP		= cp
MV		= mv
MAKE	= make

CFLAGS          = -march=armv7-a -mcpu=cortex-a9 -D_GNU_SOURCE -Wall -O2 -g -ffunction-sections -fdata-sections -Wl,--gc-sections -DCHIP_TYPE_hi3718cv100 -DCFG_HI_SDK_VERSION="HiSTBLinuxV100R002C01SPC020"
SHARECFLAGS     = -fPIC -march=armv7-a -mcpu=cortex-a9 -D_GNU_SOURCE -Wall -O2 -g -ffunction-sections -fdata-sections -Wl,--gc-sections -DCHIP_TYPE_hi3718cv100 -DCFG_HI_SDK_VERSION="HiSTBLinuxV100R002C01SPC020"
ARFLAGS         = -crs

LDFLAGSSTA      = -Wl,--start-group
LDFLAGSEND      = -Wl,--end-group

LDSHAREFLAGS  = -fPIC -shared

APP_DIR = $(STB_ROOT)/app
SAL = $(STB_ROOT)/hw
TOOL = $(STB_ROOT)/tool
SDK = $(STB_ROOT)/his_sdk

#---------------Path Macro Definition---------------


#---------------the system directory for the include file
SYS_INCLUDE =
APP_INCLUDE = -I$(SDK)/common \
							-I$(SDK)/include \
							-I$(SDK)/include/freetype2 \
							-I$(STB_ROOT)/include \
							-I$(STB_ROOT)/include/tool \
							-I$(STB_ROOT)/include/hw



#--------------the system lib ----------
LIB_DIR = -L$(SDK)/lib/static \
					-L$(SDK)/lib/share \
					-L$(STB_ROOT)/lib/slib \
          -L$(STB_ROOT)/lib/dlib

