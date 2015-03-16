export STB_ROOT = $(shell pwd)
ifndef "${hisitopdir}"
export hisitopdir = $(STB_ROOT)/..
endif
include $(STB_ROOT)/config.mak

TOOL_DIR:=$(shell find tool -maxdepth 2 -type d|grep "tool/")
SAL_DIR:=$(shell find hw -maxdepth 2 -type d|grep "hw/")
to: 
	@for dir in $(TOOL_DIR); do cd $(STB_ROOT)/$$dir; make clean; make; done

sa:
	@for dir in $(SAL_DIR); do cd $(STB_ROOT)/$$dir; make clean; make; done

all:
	@for dir in $(TOOL_DIR); do cd $(STB_ROOT)/$$dir; make clean; make; done
	@for dir in $(SAL_DIR); do cd $(STB_ROOT)/$$dir; make clean; make; done
	@cd $(STB_ROOT)/app/mbox; make clean; make
	@cd $(STB_ROOT)/app/gpio; make clean; make

clean:
	@for dir in $(TOOL_DIR); do cd $(STB_ROOT)/$$dir; make clean; done
	@for dir in $(SAL_DIR); do cd $(STB_ROOT)/$$dir; make clean; done
	@cd $(STB_ROOT)/app/mbox; make clean
	@cd $(STB_ROOT)/app/gpio; make clean