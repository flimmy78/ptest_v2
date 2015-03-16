
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libnetwork.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libnetwork.so

OBJS = udp_tool.o \
			tcp_server_tool.o \
			tcp_client_tool.o \
			network_tool.o


%.o: %.c
	$(CC) $(CFLAGS) -fPIC $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<

$(LIBOUT): $(OBJS) 
	@echo "Linking $(DLIBOUT)......"
	@$(AR) $(ARFLAGS) $(LIBOUT) $(OBJS) 
#	$(CC) $(LDSHAREFLAGS) -o $(DLIBOUT) $(OBJS)
#	$(STRIP) $(DLIBOUT)
	@echo "Linking Success!"
clean:
	$(RM) *.o *.bak $(DLIBOUT) $(LIBOUT)
