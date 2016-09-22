
#--- Locations\exec names ---#

SRC_DIR = ../app
BUILD_DIR = build
UT_EXE = AllTests.out
CPPUTEST_HOME = /Users/james/Documents/git/cpputest
MKDIR = mkdir -p

#--- C\C++ compiler ---#
CC = gcc
CPPC = g++

#--- Source code compile options ---#
SRC_INC = -I../include/ 
SRC_INC +=  -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/Device/NXP/LPC18xx/Include/ \
            -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Core/CMSIS/Include/ \
            -I../dotstack/samples/lpc18xx/common/lpc1800cmsis/Drivers/include/ \
            -I../dotstack/samples/lpc18xx/common/ \
            -I../rtos/include/ \
            -I../dotstack/samples/common/app/speed-test/ \
            -I../dotstack/samples/lpc18xx/freertos/common/ \
            -I../rtos/portable/GCC/ARM_CM3/ \
            #-I../LPCUSBLib/Drivers/USB/ \
            #-I../LPCUSBLib/Drivers/USB/Core/ \

CFLAGS = -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations

CFLAGS += -I$(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h # CppUTest memory leak

XWORKS_FLAG = -DIGNORE_CROSSWORKS=1     # don't import crossworks specific headers

#--- Specify which source files to compile ---#
C_SRC = crc.c protocol_msg.c
#C_SRC = crc.c cli.c crc.c lpcapp.c protocol_msg.c test_msgs.c CDC.c CDCDescriptors.c DualDevice.c exception.c haltest.c
EXCLUDED_C_SRC = CDC.c CDCDescriptors.c DualDevice.c exception.c haltest.c

#CPP_SRC = cppsupport.cpp hal.cpp printf-stdarg.cpp helpers.cpp
EXCLUDED_CPP_SRC = helpers.cpp app_main.cpp 

OBJS = $(addprefix $(BUILD_DIR)/, $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o))    # convert to object file names via Substitution References
#$(info [$(OBJS)])

#--- Specify which test files to compile ---#
UT_SRC = AllTests.cpp $(wildcard tests_app/*.cpp) 
$(info [$(UT_SRC)])
UT_OBJS = $(addprefix $(BUILD_DIR)/, $(UT_SRC:.cpp=.o))

#--- CppUTest compile options ---#
UT_FLAGS = -I$(CPPUTEST_HOME)/include
UT_LIBS = -L$(CPPUTEST_HOME)/cpputest_build/lib -lCppUTest -lCppUTestExt

#--- Rules ----#

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(@D)
	$(CC) -c $(CFLAGS) $(SRC_INC) $(XWORKS_FLAG) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(@D)
	$(CPPC) -c $(CFLAGS) $(SRC_INC) $(XWORKS_FLAG) $< -o $@

$(UT_OBJS): $(BUILD_DIR)/%.o: %.cpp
	@$(MKDIR) $(@D)
	$(CPPC) -c $(SRC_INC) $(UT_FLAGS) $(XWORKS_FLAG) $< -o $@

$(UT_EXE): $(UT_OBJS) $(OBJS)
	$(CPPC) $^ $(UT_LIBS) -o $(BUILD_DIR)/$@

createObjs: $(OBJS)

runUnitTests: $(UT_EXE)
	$(BUILD_DIR)/$(UT_EXE) -c 	# colour output

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean createObjs runUnitTests
