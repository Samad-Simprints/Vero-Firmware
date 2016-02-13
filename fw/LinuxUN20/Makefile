vpath %c .
vpath %c ../app

PORT = halley
CFLAGS = -D__cplusplus -Wall -O
CROSS = mips-linux-gnu-
CPP = $(CROSS)g++
CC = $(CROSS)gcc
INCPATH = -I./ -I./inc -I../include
LIBS    =  -L./lib
FLAGS   = -DUN20_APP
OBJS =	main.o protocol_msg.o test_msgs.o
SRCS =	main.cpp 

TARGET  = index-un20app

ADB_TARGET_FOLDER = /data
RESULTS = results

all : $(TARGET) 

$(TARGET) : $(OBJS)
	$(CPP) -lpthread -lsgfpamx -lsgfdu05 -lsgfplib -lm -lrt -o $@ $(OBJS) $(LIBS)

.c.o :
	$(CC) $(FLAGS) $(FLAGS) $(INCPATH) -c $<

.cpp.o :
	$(CPP) $(FLAGS) $(INCPATH) -c $<

update:
	adb push  $(TARGET)	/data

install:
	tar cvz -f index.tar.gz lib/* runtimes/* $(TARGET) InitScript start-index-un20
	adb push index.tar.gz /data
	adb push install-index-un20 /data

get-results:
		
clean :
	-rm $(TARGET)
	-rm $(OBJS)
	-rm index.tar.gz

