SRC_DIR = ../src

INC = $(addprefix -I, $(shell find $(SRC_DIR) -type d))

CC     = gcc
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 $(INC)
LDFLAGS = -lm

# ======= BST tests =======

OFILES_testbst = test_bst.o \
                 $(SRC_DIR)/BST/BST.o \
                 $(SRC_DIR)/List/List.o

# ======= PointDct BST tests =======

OFILES_testpdctbst = test_pdct.o \
                     $(SRC_DIR)/PointDct/PointDctBST.o \
                     $(SRC_DIR)/BST/BST.o \
                     $(SRC_DIR)/Point/Point.o \
                     $(SRC_DIR)/List/List.o

# ======= PointDct BST2D tests =======

OFILES_testpdctbst2d = test_pdct.o \
                       $(SRC_DIR)/PointDct/PointDctBST2d.o \
                       $(SRC_DIR)/Point/Point.o \
                       $(SRC_DIR)/List/List.o

.PHONY: all clean run

all: testbst testpdctbst testpdctbst2d

run: all
	@echo "\n>>> Running BST tests..."
	./testbst
	@echo "\n>>> Running PointDct BST tests..."
	./testpdctbst
	@echo "\n>>> Running PointDct BST2D tests..."
	./testpdctbst2d

clean:
	rm -f test_bst.o test_pdct.o testbst testpdctbst testpdctbst2d

testbst: $(OFILES_testbst)
	$(CC) -o $@ $(OFILES_testbst) $(LDFLAGS)

testpdctbst: $(OFILES_testpdctbst)
	$(CC) -o $@ $(OFILES_testpdctbst) $(LDFLAGS)

testpdctbst2d: $(OFILES_testpdctbst2d)
	$(CC) -o $@ $(OFILES_testpdctbst2d) $(LDFLAGS)

# ======= Object files =======

test_bst.o: test_bst.c
	$(CC) $(CFLAGS) -c -o $@ $<

test_pdct.o: test_pdct.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_DIR)/BST/BST.o:
	$(MAKE) -C $(SRC_DIR) BST/BST.o

$(SRC_DIR)/List/List.o:
	$(MAKE) -C $(SRC_DIR) List/List.o

$(SRC_DIR)/Point/Point.o:
	$(MAKE) -C $(SRC_DIR) Point/Point.o

$(SRC_DIR)/PointDct/PointDctBST.o:
	$(MAKE) -C $(SRC_DIR) PointDct/PointDctBST.o

$(SRC_DIR)/PointDct/PointDctBST2d.o:
	$(MAKE) -C $(SRC_DIR) PointDct/PointDctBST2d.o
