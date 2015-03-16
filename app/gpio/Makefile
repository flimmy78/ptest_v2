include $(STB_ROOT)/config.mak	 

SOURCE = $(STB_ROOT)/app/gpio

APPOUT = $(SOURCE)/gpio

APPLIB = -ldebug \
					-lframebuf \
					-ldisplay \
					-limgdec \
					-lft_Font \
					-lproduct \
					-lnetwork \
					-li2c \
					-ltemperature \
					-lrtc \
					-lgpio \
					-lserialcom

SYSLIB =	-lz \
					-ldl \
					-lm \
					-lpthread \
					-lrt \
					-lhi_common \
					-lpng \
					-lhigo \
					-lhigoadp \
					-ljpeg \
					-lhi_resampler \
					-lhi_msp \
					-lz \
					-lfreetype \
					-lhi_pes \
					-lhi_subtitle \
					-lhi_so \
					-lhi_ttx \
					-lhi_cc \
					-liconv


OBJS = main.o

APP = gpio
			
MAKES_LIST = 

%.o: %.c
	@$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<  

all: $(OBJS)
	@for each_lib in $(MAKES_LIST);do cd $$each_lib;make -f Makefile $(LIBOUT); cd ..; done
	$(LDA) $(LDFLAGSSTA) $(SYS_INCLUDE) $(APP_INCLUDE) $(LIB_DIR) $(OBJS) $(APPLIB) $(SYSLIB) $(LDFLAGSEND) -o $(APPOUT) 
	@echo "Linking Success!"
	cp -f $(STB_ROOT)/app/gpio/${APP} ${hisitopdir}/custom/usr/bin/
	cp -f $(STB_ROOT)/lib/dlib/libiconv.so.2 ${hisitopdir}/custom/usr/lib/
	
clean:all_clean	
all_clean:
	@for each_clean in $(subst \,/,$(MAKES_LIST)); do cd $$each_clean ;  make -f Makefile clean; cd .. ; done
	@-rm  -rf *.o *.~c *.bak
	@-rm -rf $(APPOUT)
	
