MKDIR = mkdir -p

#--- Locations\exec names ---#

SRC_DIR = ../app
BUILD_DIR = build
UT_EXE = AllTests.out

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

CFLAGS = -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations

CFLAGS += -DIGNORE_CROSSWORKS=1     # don't import crossworks specific headers
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h # CppUTest memory leak

#--- Specify which source files to compile ---#
C_SRC = crc.c
#C_SRC = cli.c crc.c lpcapp.c protocol_msg.c test_msgs.c
EXCLUDED_C_SRC = CDC.c CDCDescriptors.c DualDevice.c exception.c haltest.c

#CPP_SRC = app_main.cpp cppsupport.cpp hal.cpp printf-stdarg.cpp
EXCLUDED_CPP_SRC = helpers.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o))    # convert to object file names via Substitution References
#$(info [$(OBJS)])

#--- Specify which test files to compile ---#
UT_SRC = AllTests.cpp $(wildcard tests_app/*.cpp) 
$(info [$(UT_SRC)])
UT_OBJS = $(addprefix $(BUILD_DIR)/, $(UT_SRC:.cpp=.o))

#--- CppUTest compile options ---#
CPPUTEST_HOME = /Users/james/GitHub/cpputest
UT_FLAGS = -I$(CPPUTEST_HOME)/include
UT_LIBS = -L$(CPPUTEST_HOME)/cpputest_build/lib -lCppUTest -lCppUTestExt

#--- Rules ----#

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(@D)
	$(CC) -c $(CFLAGS) $(SRC_INC) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(@D)
	$(CPPC) -c $(CFLAGS) $(SRC_INC) $< -o $@

$(UT_OBJS): $(BUILD_DIR)/%.o: %.cpp
	@$(MKDIR) $(@D)
	$(CPPC) -c $(SRC_INC) $(UT_FLAGS) $< -o $@

$(UT_EXE): $(UT_OBJS) $(OBJS)
	$(CPPC) $^ $(UT_LIBS) -o $(BUILD_DIR)/$@

runUnitTests: $(UT_EXE)
	$(BUILD_DIR)/$(UT_EXE) -c 	# colour output

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean createObjs runUnitTests
