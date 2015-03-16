
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libproduct.a
DLIBOUT = $(STB_ROOT)/lib/dlib/libproduct.so

OBJS = product_net.o product_block_device.o product_gpio.o product_info.o

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
