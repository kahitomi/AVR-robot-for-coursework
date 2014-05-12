# CROSS   ?=avr-
# CC      :=$(CROSS)gcc
# CXX     :=$(CROSS)g++
# LD      :=$(CROSS)g++
# SIZE    :=$(CROSS)size
# OBJCOPY :=$(CROSS)objcopy
# OBJDUMP :=$(CROSS)objdump

# TARGET=a
# MMCU?=

# SOURCES=$(wildcard *.cpp) $(wildcard *.c)
# INCLUDES=

# SETTINGS=settings.h

# OBJECTS=$(SOURCES:.cpp=.o)
# OBJECTS:=$(OBJECTS:.c=.o)

# CSTD?=c99
# COPT=-O2 -fdata-sections -ffunction-sections
# CFLAGS=-mmcu=$(MMCU) -std=$(CSTD) $(COPT) -Wall
# CFLAGS+=$(addprefix -I,$(INCLUDES))
# CFLAGS+=-include "$(SETTINGS)"

# CXXSTD?=c++98
# CXXOPT=$(COPT) -fno-exceptions -fno-rtti
# CXXFLAGS=-mmcu=$(MMCU) -std=$(CXXSTD) $(CXXOPT) -Wall
# CXXFLAGS+=$(addprefix -I,$(INCLUDES))
# CXXFLAGS+=-include "$(SETTINGS)"

# LDFLAGS=-mmcu=$(MMCU) -Wl,--gc-sections -Wl,-Map=$(TARGET).map,--cref

# .PHONY: all
# all: $(TARGET).hex $(TARGET).lst

# $(TARGET).elf: $(OBJECTS)
# 	$(LD) $(LDFLAGS) $^ -lm -o $@

# $(TARGET).hex: $(TARGET).elf
# 	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

# $(TARGET).bin: $(TARGET).elf
# 	$(OBJCOPY) -O binary -R .eeprom -R .fuse -R .lock -R .signature $< $@

# %.o: %.cpp
# 	$(CXX) -o $@ $(CXXFLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -c

# %.o: %.c
# 	$(CC) -o $@ $(CFLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -c

# $(TARGET).lst: $(TARGET).elf
# 	$(OBJDUMP) -h -S $< > $@

#############
# Settings
#############
BOARD:=atmega644p
TYPE:=m644p
CLOCK:=12000000

#############
# Variables
#############
AVR_GCC:=avr-gcc
AVR_OBJCOPY:=avr-objcopy
AVR_DUDE:=avrdude

AVR_GCC_OPTS:=$(AVR_GCC_OPTS) -mmcu=$(BOARD) -DF_CPU=$(CLOCK) -ggdb -Wall -Os
AVR_OBJCOPY_OPTS:=$(AVR_OBJCOPY_OPTS)
SOURCE:=$(wildcard *.c)

OBJDIR:=build/
OBJECTS:=$(addprefix $(OBJDIR),$(SOURCE:.c=.o))

DIR:=$(lastword $(subst /, ,$(CURDIR)))
OUTPUT_NAME:=$(addsuffix .hex,$(addprefix $(OBJDIR),$(DIR)))

#############
# Rules
#############
$(OBJDIR)%.o: %.c | $(OBJDIR).dirtag
	@echo "Compiling $< into $@"
	@$(AVR_GCC) $(AVR_GCC_OPTS) -c $< -o $@

$(OUTPUT_NAME:.hex=.elf): $(OBJECTS)
	@echo "Linking $^ into $@"
	@$(AVR_GCC) $(AVR_GCC_OPTS) -o $@ $^

$(OUTPUT_NAME): $(OUTPUT_NAME:.hex=.elf)
	@echo "Creating hex $@"
	@$(AVR_OBJCOPY) $(AVR_OBJCOPY_OPTS) -O ihex $^ $@

build: $(OUTPUT_NAME)
	@echo "Build $(DIR) completed successfully."

.PHONY:upload
upload: $(OUTPUT_NAME)
	@echo "Uploading hex $<"
	@$(AVR_DUDE) -c usbasp -p $(TYPE) -U flash:w:$^

$(OBJDIR).dirtag:
	@echo "Creating build directory $(OBJDIR)"
	@-mkdir $(OBJDIR)
	@touch $@

.PHONY:clean
clean:
	@echo "Deleting all compiled files and removing build directory $(OBJDIR)"
	@rm -rf $(OBJDIR)

#---------------- Debugging Options ----------------

# For simulavr only - target MCU frequency.
DEBUG_MFREQ = $(F_CPU)

# Set the DEBUG_UI to either gdb or insight.
DEBUG_UI = gdb                
# 注意这里，如果你喜欢consol程序就用gdb
#DEBUG_UI = insight           # 如果喜欢 GUI界面就用 insight

# Set the debugging back-end to either avarice, simulavr.
#DEBUG_BACKEND = avarice     # 如果你有JTAG 等工具可用avarice，我没有，所以用simulabr模拟调试。  PS：好像支持“龙”哦，等会儿试下。
DEBUG_BACKEND = simulavr

# GDB Init Filename.
GDBINIT_FILE = __avr_gdbinit

# When using avarice settings for the JTAG
# JTAG_DEV = /dev/com1

# Debugging port used to communicate between GDB / avarice / simulavr.
DEBUG_PORT = 4242

# Debugging host used to communicate between GDB / avarice / simulavr, normally
#     just set to localhost unless doing some sort of crazy debugging when 
#     avarice is running on a different computer.
DEBUG_HOST = localhost