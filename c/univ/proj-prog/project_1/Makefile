# Makefile
# By: Pavlov Aleksandr s2400691
# INFO0030: Projet 1

TARGET = pnm

CC = gcc
CFLAGS = --std=c99 --pedantic -Wall -W -Wmissing-prototypes
LD = gcc
LDFLAGS =

SRC_DIR = src
OBJ_DIR = bin

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/pnm.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(SRC_DIR)/main.c -o $@ $(CFLAGS)

$(OBJ_DIR)/pnm.o: $(SRC_DIR)/pnm.c $(SRC_DIR)/pnm.h
	$(CC) -c $(SRC_DIR)/pnm.c -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

test: $(TARGET)
	./$(TARGET) -f pbm -i test_image/test.pbm -o test_image/result.pbm
	./$(TARGET) -f pgm -i test_image/test.pgm -o test_image/result.pgm
	./$(TARGET) -f ppm -i test_image/test.ppm -o test_image/result.ppm

git:
	git pull
	git add .
	git commit -m "Makefile push"
	git push
