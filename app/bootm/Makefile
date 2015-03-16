include $(STB_ROOT)/config.mak	 

SOURCE = $(STB_ROOT)/app/bootm

APPOUT = $(SOURCE)/bootm

APPLIB = -lcrc32

SYSLIB =	-ldl \
				-lm \
				-lpthread

				
OBJS = bootm.o
			
MAKES_LIST = 

%.o: %.c
	@$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<  

all: $(OBJS)
	@for each_lib in $(MAKES_LIST);do cd $$each_lib;make -f Makefile $(LIBOUT); cd ..; done
	$(LDA) $(LDFLAGSSTA) $(SYS_INCLUDE) $(APP_INCLUDE) $(LIB_DIR) $(OBJS) $(APPLIB) $(SYSLIB) $(LDFLAGSEND) -o $(APPOUT) 
	cp $(APPOUT) /home/lizz/work/
	@echo "Linking Success!"

clean:all_clean	
all_clean:
	@for each_clean in $(subst \,/,$(MAKES_LIST)); do cd $$each_clean ;  make -f Makefile clean; cd .. ; done
	@-rm  -rf *.o *.~c *.bak
	@-rm -rf $(APPOUT)
	
