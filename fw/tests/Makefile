

#--- Locations ---#

SRC_DIR = ../app
BUILD_DIR = build

#--- C compiler ---#
CC = gcc
INCUDES = -I../include/ 
INCUDES +=  -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/Device/NXP/LPC18xx/Include/ \
            -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/CMSIS/Include/ \
            -I../rtos/include/ \
            -I../dotstack/samples/common/app/speed-test/ \
            -I../dotstack/samples/lpc18xx/freertos/common/ -I../rtos/portable/GCC/ARM_CM3/ \

CFLAGS = -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations

#--- Specify which source files to compile ---#
C_SRC = crc.c #lpcapp.c
OBJS = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(C_SRC)))

#--- Rules ----#
createObjs: $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(INCUDES) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o

.PHONY: clean createObjs
