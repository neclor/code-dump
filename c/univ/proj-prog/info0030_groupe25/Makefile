TARGET = five

SRC_DIR	= source
OBJ_DIR = build

INC = $(addprefix -I, $(shell find $(SRC_DIR) -type d))

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
HEDS = $(shell find $(SRC_DIR) -type f -name "*.h")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -g --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes `pkg-config --cflags gtk+-2.0` $(INC)
LD = gcc
LDFLAGS = `pkg-config --libs gtk+-2.0`

all: $(TARGET)

five: $(OBJS) $(LIBS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(TARGET): $(OBJS) $(LIBS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

doc:
	@doxygen Doxyfile

rapport:
	@cd rapport && pdflatex -interaction=nonstopmode rapport.tex
	@cd rapport && pdflatex -interaction=nonstopmode rapport.tex

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

git:
	@git pull
	@git add .
	@git commit -m Update
	@git push

tar:
	@mkdir -p Groupe33
	@cp -r $(SRC_DIR) rapport doc Makefile Doxyfile Groupe33/
	@tar -czvf five_33.tar.gz  Groupe33/
	@rm -rf Groupe33/

.PHONY: all $(TARGET) clean doc rapport git tar
