CC=gcc
LIBCORESRC=./ReturnChannel_API_Core
LIBHOSTSRC=./ReturnChannel_API_Host
LIBSHELLSRC=./ReturnChannel_Shell_Host
SRC=.
BIN=.
EXECUTE_NAME := ReturnChannel_Sample

all:	$(BIN)/$(EXECUTE_NAME)

$(BIN)/ReturnChannel_Sample:  $(SRC)/ReturnChannel_Host_TestKit.c $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c
	$(CC) $(SRC)/ReturnChannel_Host_TestKit.c $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c -o  $(BIN)/$(EXECUTE_NAME) -lpthread



returnchannel.so:
	$(CC) $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c $(SRC)/ReturnChannelApi.c -lpthread -fPIC -shared -o libreturnchannel.so

sample:
	#gcc -I$(LIBCORESRC) $(SRC)/sample.c -L./ -lreturnchannel -lstartservice -lpthread -lm -o sample
	$(CC) $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c $(SRC)/sample.c $(SRC)/ReturnChannelApi.c -I$(LIBCORESRC) -L./ -lstartservice -lpthread -lm  -o sample


vacron:
	$(CC) $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c $(SRC)/vacron.c $(SRC)/ReturnChannelApi.c -I$(LIBCORESRC) ./libstartservice.so -lpthread -lm  -o vacron

test:	
	rm -f test
#	gcc -I$(LIBCORESRC) $(SRC)/test.c -L./ -lreturnchannel -lstartservice -lpthread -lm -o test

	$(CC) $(SRC)/rs232.c $(LIBHOSTSRC)/ReturnChannel_Cmd_Host.c $(LIBHOSTSRC)/ReturnChannel_User_Host.c $(LIBCORESRC)/ReturnChannel_Cmd.c $(LIBCORESRC)/ReturnChannel_User.c $(LIBSHELLSRC)/ReturnChannel_Shell.c $(SRC)/ReturnChannelApi.c $(SRC)/test.c -I$(LIBCORESRC) -L./ -lstartservice -lpthread -lm  -o test
clean:
	@-rm -f *.o *.d $(EXECUTE_NAME) *.so sample

.PHONY: clean all
