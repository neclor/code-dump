TARGET = out

SRC_DIR	= test work # test/filededup_test src
OBJ_DIR = build

INC = $(addprefix -I, $(shell find $(SRC_DIR) -type d))

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c") test/main_test.c
HEDS = $(shell find $(SRC_DIR) -type f -name "*.h")
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g --std=c11 --pedantic -Wall -O3 -W -Wextra -Wmissing-prototypes -Werror $(INC)
LD = gcc
LDFLAGS =

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all $(TARGET) clean
