CC=gcc

ASCII_ART_TOOL = tool/AsciiArtTool

RLEList = /home/mtm/public/2122b/ex1/RLEList

MAIN = tool/main

OBJS = $(ASCII_ART_TOOL).o RLEList.o $(MAIN).o

EXEC = AsciiArtTool

DEBUG =#now empty. assign -g for debug

CFLAG = -std=c99 -Wall -Werror -I/home/mtm/public/2122b/ex1

#CFLAGS = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG) $(OBJS) $(CFLAG) -o $@
RLEList.o : RLEList.c RLEList.h
	$(CC) -c $(CFLAG) $(DEBUG) $*.c

$(ASCII_ART_TOOL).o : $(ASCII_ART_TOOL).c $(ASCII_ART_TOOL).h RLEList.c $(RLEList).h
	$(CC) -c $(CFLAG) $(DEBUG) $*.c -o $(ASCII_ART_TOOL).o

$(MAIN).o: $(MAIN).c $(ASCII_ART_TOOL).c $(ASCII_ART_TOOL).h RLEList.c $(RLEList).h
	$(CC) -c $(CFLAG) $(DEBUG) $*.c -o $(MAIN).o

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)