

#--- Locations ---#

SRC_DIR = ../app
BUILD_DIR = build

#--- C\C++ compiler ---#
CC = gcc
CPPC = g++
INCUDES = -I../include/ 
INCUDES +=  -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/Device/NXP/LPC18xx/Include/ \
            -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/CMSIS/Include/ \
            -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Drivers/include/ \
            -I../dotstack/samples/lpc18xx/common/ \
            -I../rtos/include/ \
            -I../dotstack/samples/common/app/speed-test/ \
            -I../dotstack/samples/lpc18xx/freertos/common/ \
            -I../rtos/portable/GCC/ARM_CM3/ \

CFLAGS = -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -DIGNORE_CROSSWORKS=1     # don't import crossworks specific headers

#--- Specify which source files to compile ---#
C_SRC = cli.c crc.c lpcapp.c protocol_msg.c test_msgs.c
EXCLUDED_C_SRC = CDC.c CDCDescriptors.c DualDevice.c exception.c haltest.c

CPP_SRC = app_main.cpp cppsupport.cpp hal.cpp printf-stdarg.cpp
EXCLUDED_CPP_SRC = helpers.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o))    # convert to object file names via Substitution References
#$(info [$(OBJS)])

#--- Rules ----#
createObjs: $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(INCUDES) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPPC) -c $(CFLAGS) $(INCUDES) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o

.PHONY: clean createObjs
