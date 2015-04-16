# Compile the STM32F2xx Standard Peripheral Library
# 3
#Setup cluster
DEVICE_FAMILY = STM32F2XX
DEVICE_HSE = 25000000U


# Write here library dir
LIBDIR=../usb



# Make host lib
OTG_CFILES = usbd_core.c usbd_ioreq.c usbd_req.c

DEVICE_CFILES = usb_core.c usb_dcd.c usb_dcd_int.c

#Job
TARGET_ARCH = -mcpu=cortex-m3 -mthumb
TARGET_OPTS = -O2 -finline -finline-functions-called-once
WARNINGS = -Wall -W -Wshadow -Wwrite-strings -Winline
INCLUDE_DIRS = -I ../inc/ -I .
F_NO_STRICT_ALIASING = -fno-strict-aliasing

DEFINE_LIST = -DUSE_STDPERIPH_DRIVER -D$(DEVICE_FAMILY) -DHSE_VALUE=$(DEVICE_HSE)

COMPILE_OPTS = $(WARNINGS) $(TARGET_ARCH) $(TARGET_OPTS) $(MESSAGES) $(F_NO_STRICT_ALIASING) $(INCLUDE_DIRS) $(DEFINE_LIST) -ffunction-sections

CC = arm-none-eabi-gcc
CFLAGS = $(COMPILE_OPTS)


AR = arm-none-eabi-ar
ARFLAGS = cr

LIB_OUT = libusb$(shell echo "$(DEVICE_FAMILY)" | tr A-Z a-z).a



SOURCES_C = \
        $(addprefix $(LIBDIR)/src/STM32_USB-FS-Device_Driver/, $(DEVICE_CFILES)) \
	$(addprefix $(LIBDIR)/src/STM32_USB_OTG_Driver/,	  $(OTG_CFILES))
 

LIB_OBJS_C = \
  $(subst src/,obj/,$(patsubst %.c, %.o, $(SOURCES_C)))
 


.PHONY: all
all: mkbuilddir $(LIB_OUT)
#all:
#	print $(SOURCES_C)

mkbuilddir:
	mkdir $(LIBDIR)/obj
	mkdir $(LIBDIR)/obj/STM32_USB-FS-Device_Driver
	mkdir $(LIBDIR)/obj/STM32_USB_OTG_Driver

$(LIB_OUT): $(LIB_OBJS_C) $(LIB_OBJS_S)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS_C) 




$(LIB_OBJS_C):
	$(CC) -o $@ -c $(subst obj/,src/,$(subst .o,.c,$@)) $(CFLAGS) 


.PHONY: clean
clean:
	rm -rf $(LIBDIR)/obj
	rm $(LIB_OUT)
