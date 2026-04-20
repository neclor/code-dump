TARGET = filtre

SRC_DIR	= src
OBJ_DIR = build
LIB_DIR	= lib

INC = $(addprefix -I, $(shell find $(SRC_DIR) $(LIB_DIR) -type d))

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBS = $(LIB_DIR)/pnm/libpnm.a

# Tests
TEST = pnm_tests
TST_DIR = test
TST_INC = $(addprefix -I, $(shell find $(TST_DIR) -type d))
TSTS = $(shell find $(TST_DIR) -type f -name "*.c")
TST_OBJS = $(TSTS:$(TST_DIR)/%.c=$(TST_DIR)/$(OBJ_DIR)/%.o)
TST_CFLAGS = --std=c99 $(INC) $(TST_INC)

CC = gcc
CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes $(INC)
LD = gcc
LDFLAGS = -lm

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(LIB_DIR)/%.a:
	@make -C $(dir $@)

pnm_librairie: $(LIBS)

$(TEST): $(TST_OBJS) $(LIBS) $(OBJ_DIR)/filter.o
	$(LD) $^ -o $@ $(LDFLAGS)

$(TST_DIR)/$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(TST_CFLAGS)

pnm_clean:
	@make -C $(LIB_DIR)/pnm clean

doc:
	@doxygen Doxyfile

clean:
	@make pnm_clean
	@rm -rf $(OBJ_DIR) $(TARGET) $(TST_DIR)/$(OBJ_DIR) $(TEST)

my_test: $(TARGET)
	./$< -i test_image/valid_image.ppm -o a.ppm
	./$< -i test_image/valid_image.ppm -f retournement -o a.ppm
	./$< -i test_image/valid_image.ppm -f monochrome -p r -o a.ppm
	./$< -i test_image/valid_image.ppm -f negatif -o a.ppm
	./$< -i test_image/valid_image.ppm -f gris -p 1 -o a.pgm
	./$< -i test_image/valid_image.ppm -f NB -p 128 -o a.pbm
	rm a.pbm a.pgm a.ppm

git:
	@git pull
	@git add .
	@git commit -m Update
	@git push

tar:
	@tar -czvf filtres.tar.gz $(SRC_DIR) $(LIB_DIR) $(TST_DIR) test_image Makefile Doxyfile

.PHONY: all $(TARGET) $(TEST) pnm_librairie pnm_clean doc clean my_test git tar
