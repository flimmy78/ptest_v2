
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libmsg.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libmsg.so
OBJS = IPCComunicationABS.o
	
	
%.o: %.c
	$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<
#	@$(CC) $(CFLAGS) -fPIC $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<

$(LIBOUT): $(OBJS) 
#	@echo "Linking $(LIBOUT)......"	
	$(AR) $(ARFLAGS) $(LIBOUT) $(OBJS)
#	@$(CC) $(LDSHAREFLAGS) -o $(DLIBOUT) $(OBJS)
#	@$(STRIP) $(DLIBOUT)
	@echo "$(DLIBOUT) Linking Success!"	                                              
clean:
	$(RM) *.o *.bak $(LIBOUT) $(DLIBOUT)
