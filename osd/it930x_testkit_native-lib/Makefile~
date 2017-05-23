CC = gcc
CFLAGS = -I ./
LFLAGS = -lm  
RM = rm -f

TARGETS = it930x_testkit_native
OBJS = DTVAPI.o it930x_testkit_native.o

all: $(OBJS) $(TARGETS)
	@echo
	@echo "--------------------------------------------------------------------------------"
	@echo " please call ./it930x_testkit_native for Endeavour"
	@echo "--------------------------------------------------------------------------------"
	@echo

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

.o:
	$(CC) $(LFLAGS) -o $@ DTVAPI.o it930x_testkit_native.o -lm

clean:
	$(RM) $(TARGETS) core* *.o *.d *.so .depend

service.so:
	gcc $(CFLAGS) -lm DTVAPI.c it930x_testkit_native.c -fPIC -shared -o libstartservice.so


-include $(wildcard *.d) dummy

