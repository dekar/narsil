# Compile the STM32F2xx Standard Peripheral Library
# 3
#Setup cluster
DEVICE_FAMILY = STM32F2XX
DEVICE_HSE = 25000000U


#Calculating path
STD_PERIPH_PATH = periph
CORE_PATH = core
ASM_PATH = asm
SYSTEM_PATH = system

#Job
TARGET_ARCH = -mcpu=cortex-m3 -mthumb
TARGET_OPTS = -O2 -finline -finline-functions-called-once
WARNINGS = -Wall -W -Wshadow -Wwrite-strings -Winline
INCLUDE_DIRS = -I ./inc/
F_NO_STRICT_ALIASING = -fno-strict-aliasing

DEFINE_LIST = -DUSE_STDPERIPH_DRIVER -D$(DEVICE_FAMILY) -DHSE_VALUE=$(DEVICE_HSE)

COMPILE_OPTS = $(WARNINGS) $(TARGET_ARCH) $(TARGET_OPTS) $(MESSAGES) $(F_NO_STRICT_ALIASING) $(INCLUDE_DIRS) $(DEFINE_LIST) -ffunction-sections

CC = arm-none-eabi-gcc
CFLAGS = $(COMPILE_OPTS)

#AS = $(CC) -x assembler-with-cpp -c $(TARGET_ARCH)
AS = $(CC) -x assembler-with-cpp

ASFLAGS = $(COMPILE_OPTS)

AR = arm-none-eabi-ar
ARFLAGS = cr

LIB_OUT = lib$(shell echo "$(DEVICE_FAMILY)" | tr A-Z a-z).a

SOURCES_C = \
 $(wildcard ./src/$(STD_PERIPH_PATH)/*.c ./src/$(CORE_PATH)/*.c ./src/$(SYSTEM_PATH)/*.c)

SOURCES_ASM = \
  ./src/$(ASM_PATH)/startup_$(shell echo "$(DEVICE_FAMILY)" | tr A-Z a-z).s

LIB_OBJS_C = \
  $(subst src/,obj/,$(patsubst %.c, %.o, $(SOURCES_C)))
LIB_OBJS_S = \
  $(subst src/,obj/,$(patsubst %.s, %.o, $(SOURCES_ASM)))
 


.PHONY: all
all: mkbuilddir $(LIB_OUT)
#all:
#	print $(SOURCES_C)

mkbuilddir:
	mkdir -p obj/$(STD_PERIPH_PATH)
	mkdir -p obj/$(CORE_PATH)
	mkdir -p obj/$(ASM_PATH)
	mkdir -p obj/$(SYSTEM_PATH)

$(LIB_OUT): $(LIB_OBJS_C) $(LIB_OBJS_S)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS_C) $(LIB_OBJS_S)




$(LIB_OBJS_C):
	$(CC) -o $@ -c $(subst obj/,src/,$(subst .o,.c,$@)) $(CFLAGS) 

$(LIB_OBJS_S):
	$(AS) -o $@ -c $(subst obj/,src/,$(subst .o,.s,$@)) $(CFLAGS) 


.PHONY: clean
clean:
	rm -rf obj/*
	
