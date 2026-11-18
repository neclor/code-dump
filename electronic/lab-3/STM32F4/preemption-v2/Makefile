#
#  INFO0064 - Embedded systems
#
#  Makefile for SMT32F469 demo
#
#  Usage:
#
#    make       : Builds the project
#    make clean : Cleans the project directory
#    make flash : Programs the board using OpenOCD
#

## Local configuration #########################################################

LIBOPENCM3-DIR = ../libopencm3/
FREERTOS-DIR = ../FreeRTOS-LTS/
LIBC-DIR = /usr/lib/arm-none-eabi/newlib/thumb/v7e-m+fp/hard/
LIBGCC-DIR = /usr/lib/gcc/arm-none-eabi/latest/thumb/v7e-m+fp/hard/

PGM-CONFIG = /usr/share/openocd/scripts/board/stm32f429disc1.cfg

## Project files ###############################################################

EXEC = main
SRCS = main.c

################################################################################

INCLUDE-DIR = /usr/lib/arm-none-eabi/include
GCC-INCLUDE-DIR = /usr/lib/gcc/arm-none-eabi/latest/include
FREERTOS-INCLUDE-DIR = $(FREERTOS-DIR)/FreeRTOS/FreeRTOS-Kernel/include
FREERTOS-PORT-DIR = $(FREERTOS-DIR)/FreeRTOS//FreeRTOS-Kernel/portable/GCC/ARM_CM4F

PLATFORM = stm32f469

CC = arm-none-eabi-gcc

CFLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -Os -MD
CFLAGS += -Wall -Wextra -Wmissing-prototypes \
          -Wstrict-prototypes  -Wredundant-decls \
          -Wimplicit-function-declaration
CFLAGS += -I$(INCLUDE-DIR) -I$(GCC-INCLUDE-DIR) \
          -I$(LIBOPENCM3-DIR)/include -I$(FREERTOS-INCLUDE-DIR)/ \
          -I$(FREERTOS-PORT-DIR) -iquote . -iquote utils/

LDFLAGS = --static -nostartfiles -T$(LD-SCRIPT) -Wl,-Map=$(MAP-FILE) \
          -Wl,--gc-sections -L$(LIBOPENCM3-DIR)/lib -L$(LIBC-DIR) \
          -L$(LIBGCC-DIR)

LIBS    = -lc_nano -lnosys -lopencm3_stm32f4 -lm

FREERTOS-SRC = freertos/heap_4.c freertos/list.c freertos/port.c \
               freertos/tasks.c freertos/queue.c

ALL-SRCS = $(SRCS) utils/sdram.c utils/lcd.c utils/gfx.c utils/fonts.c \
           utils/viewport.c utils/delay.c utils/utils.c utils/syscalls.c \
	   $(FREERTOS-SRC)

LD-SCRIPT = $(PLATFORM).ld
MAP-FILE  = $(EXEC).map

OBJS = ${ALL-SRCS:.c=.o}
DEPS = ${ALL-SRCS:.c=.d}

.EXTRA_PREREQS := Makefile

.PHONY: all clean flash

all: $(EXEC)

-include $(DEPS)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) \
	-o $(EXEC)
clean:
	-@rm -f $(OBJS) $(DEPS) $(EXEC) $(MAP-FILE)

flash: $(EXEC)
	openocd	-f $(PGM-CONFIG) -c "program $(EXEC) verify reset exit"
