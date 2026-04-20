# ======= Make ======= #

TARGET = prefixe_suffixe

SRC_DIR	= .
OBJ_DIR = build

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
INCS = $(shell find $(SRC_DIR) -type d)

I_INC = $(addprefix -I, $(INCS))

CC = gcc
CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes $(I_INC)
LD = gcc
LDFLAGS = -lm

# ======= Rules ======= #

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCS)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm -rf $(OBJ_DIR) course_tableau_test course_liste_test

git:
	@git pull
	@git add .
	@git commit -m Update
	@git push

tar:
	@tar -czvf ../pref_suf-groupe33.tar.gz ../code ../rapport

.PHONY: all clean git tar
