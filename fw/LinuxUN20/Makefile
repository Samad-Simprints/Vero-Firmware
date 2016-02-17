vpath %c .
vpath %c ../app

PORT = halley
CFLAGS = -D__cplusplus -Wall -O
CROSS = mips-linux-gnu-
CPP = $(CROSS)g++
CC = $(CROSS)gcc
INCPATH = -I./ -I./inc -I../include -I./libwsq-mips-packaged/raw-wsq-mips/include
LIBS    =  -L./lib -L./libwsq-mips-packaged/libpng-1.6.20/.libs -L./libwsq-mips-packaged/zlib-1.2.8 -L./libwsq-mips-packaged/raw-wsq-mips
FLAGS   = -DUN20_APP
OBJS =	main.o compress.o protocol_msg.o test_msgs.o
#LDFLAGS = 
TARGET  = index-un20app

STATIC_LIBS =  ./libwsq-mips-packaged/libpng-1.6.20/.libs/libpng16.a
STATIC_LIBS += ./libwsq-mips-packaged/zlib-1.2.8/libz.a
STATIC_LIBS += ./libwsq.a

ADB_TARGET_FOLDER = /data
RESULTS = results

#	$(CPP) $(LDFLAGS) $(LIBS) -Bstatic $(STATIC_LIBS) -lpthread -lsgfpamx -lsgfdu05 -lsgfplib -lm -lrt -o $@ $(OBJS) $(STATIC_LIBS)

all : $(TARGET) 

$(TARGET) : $(OBJS)
	$(CPP) $(OBJS) $(LDFLAGS) $(LIBS) -Wl,-Bstatic -lpng16 -lz -lwsq -Wl,-Bdynamic -lpthread -lsgfpamx -lsgfdu05 -lsgfplib -lm -lrt -o $@

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

