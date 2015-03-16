
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libimgdec.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libimgdec.so

OBJS = decjpeg.o \
			decpng.o


%.o: %.c
#	@$(CC) $(CFLAGS) -fPIC $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<
	@$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<
	
$(LIBOUT): $(OBJS) 
#	@echo "Linking $(DLIBOUT)......"
#	@$(CC) $(LDSHAREFLAGS) -o $(DLIBOUT) $(OBJS)
	@$(AR) $(ARFLAGS) $(LIBOUT) $(OBJS)
	@echo "$(LIBOUT) Linking Success!!!!!!"

clean:
	@$(RM) *.o *.bak $(DLIBOUT) $(LIBOUT)
