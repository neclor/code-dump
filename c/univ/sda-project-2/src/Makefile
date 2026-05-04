SRC_DIR	= .

INC = $(addprefix -I, $(shell find $(SRC_DIR) -type d))


OFILES_testpdlist = testcputime.o PointDct/PointDctList.o Point/Point.o List/List.o
OFILES_testpdbst = testcputime.o PointDct/PointDctBST.o Point/Point.o List/List.o BST/BST.o
OFILES_testpdbst2d = testcputime.o PointDct/PointDctBST2d.o Point/Point.o List/List.o
OFILES_taxi = testtaxi.o PointDct/PointDctBST2d.o Point/Point.o List/List.o BST/BST.o

TARGET_testpdlist = testpdlist
TARGET_testpdbst = testpdbst
TARGET_testpdbst2d = testpdbst2d
TARGET_taxi = testtaxi

CC = gcc
CFLAGS = -W -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 $(INC)

.PHONY: all clean run archive

LDFLAGS = -lm

all: $(TARGET_testpdlist) $(TARGET_testpdbst) $(TARGET_testpdbst2d) $(TARGET_taxi)

clean:
	rm -f $(OFILES_testpdlist) $(OFILES_testpdbst) $(OFILES_testpdbst2d) $(OFILES_taxi) $(OFILES_testbst) $(OFILES_testbst2d)
	rm -f $(TARGET_testpdlist) $(TARGET_testpdbst) $(TARGET_testpdbst2d) $(TARGET_taxi)

run: $(TARGET_testpdlist) $(TARGET_testpdbst) $(TARGET_testpdbst2d)
	./$(TARGET_testpdlist) 1000000 10000 0.01
	./$(TARGET_testpdbst) 1000000 10000 0.01
	./$(TARGET_testpdbst2d) 1000000 10000 0.01

archive:
	zip -j fichiers-p2-2025-2026.zip BST/BST.c BST/BST.h declaration.txt List/List.c List/List.h Makefile Point/Point.c Point/Point.h PointDct/PointDct.h PointDct/PointDctBST.c PointDct/PointDctBST2d.c PointDct/PointDctList.c testcputime.c testtaxi.c

$(TARGET_testpdlist): $(OFILES_testpdlist)
	$(CC) -o $(TARGET_testpdlist) $(OFILES_testpdlist) $(LDFLAGS)

$(TARGET_testpdbst): $(OFILES_testpdbst)
	$(CC) -o $(TARGET_testpdbst) $(OFILES_testpdbst) $(LDFLAGS)

$(TARGET_testpdbst2d): $(OFILES_testpdbst2d)
	$(CC) -o $(TARGET_testpdbst2d) $(OFILES_testpdbst2d) $(LDFLAGS)

$(TARGET_taxi): $(OFILES_taxi)
	$(CC) -o $(TARGET_taxi) $(OFILES_taxi) $(LDFLAGS)

BST/BST.o: BST/BST.c BST/BST.h List/List.h
List/List.o: List/List.c List/List.h
Point/Point.o: Point/Point.c Point/Point.h
PointDct/PointDctBST.o: PointDct/PointDctBST.c PointDct/PointDct.h List/List.h Point/Point.h BST/BST.h
PointDct/PointDctBST2d.o: PointDct/PointDctBST2d.c PointDct/PointDct.h List/List.h Point/Point.h
PointDct/PointDctList.o: PointDct/PointDctList.c PointDct/PointDct.h List/List.h Point/Point.h

testcputime.o: testcputime.c PointDct/PointDct.h List/List.h Point/Point.h
testtaxi.o: testtaxi.c PointDct/PointDct.h List/List.h Point/Point.h
