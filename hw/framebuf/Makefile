
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libframebuf.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libframebuf.so

OBJS = framebuf.o \
			logic_framebuf.o
	
%.o: %.c
	@$(CC) $(CFLAGS) -fPIC $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<

$(LIBOUT): $(OBJS) 
#	@echo "Linking $(LIBOUT)......"
	$(AR) $(ARFLAGS) -o $(LIBOUT) $(OBJS)
#	$(STRIP) $(DLIBOUT)
	@echo "$(LIBOUT) Linking Success!"                                          
clean:
	@$(RM) *.o *.bak $(LIBOUT) $(DLIBOUT)
