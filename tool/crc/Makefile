
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libcrc32.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libcrc32.so
OBJS = crc32.o

%.o: %.c
	$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<

$(LIBOUT): $(OBJS) 
	@echo "Linking $(LIBOUT)......"	
	$(AR) $(ARFLAGS) -o $(LIBOUT) $(OBJS)
	@echo "Linking Success!"	                                              
clean:
	$(RM) *.o *.bak $(LIBOUT)
