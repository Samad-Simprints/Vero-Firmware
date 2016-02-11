vpath %c .
vpath %c ../app

PORT = halley
CFLAGS = -D__cplusplus -Wall -O
CROSS = mips-linux-gnu-
CPP = $(CROSS)g++
CC = $(CROSS)gcc
INCPATH = -I./ -I../inc -I../include
LIBS    =  -L./lib
FLAGS   = -DUN20_APP
OBJS =	main.o protocol_msg.o test_msgs.o
SRCS =	main.cpp 

TARGET  = index_un20app

ADB_TARGET_FOLDER = /data
RESULTS = results

all : $(TARGET) 

$(TARGET) : $(OBJS)
	$(CPP) -lpthread -lsgfpamx -lsgfdu05 -lsgfplib -lm -lrt -o $@ $(OBJS) $(LIBS)

.c.o :
	$(CC) $(FLAGS) $(FLAGS) $(INCPATH) -c $<

.cpp.o :
	$(CPP) $(FLAGS) $(INCPATH) -c $<

install:
	adb push  $(TARGET)	/data

get-results:
		
clean :
	-rm $(TARGET)
	-rm $(OBJS)

