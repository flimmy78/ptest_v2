
include $(STB_ROOT)/config.mak

LIBOUT = $(STB_ROOT)/lib/slib/libserialcom.a

OBJS = uart.o
	
	
%.o: %.c
	$(CC) $(CFLAGS) $(SYS_INCLUDE) $(APP_INCLUDE) -c -o $@ $<

$(LIBOUT): $(OBJS) 
	@echo "Linking $(LIBOUT)......"	
	$(AR) $(ARFLAGS) $(LIBOUT) $(OBJS)
	@echo "Linking Success!"	                                              
clean:
	$(RM) *.o *.~c *.bak $(LIBOUT)
