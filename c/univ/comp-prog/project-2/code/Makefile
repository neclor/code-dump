# ======= Make ======= #

SRC_DIR	= src
OBJ_DIR = build

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
INCS = $(shell find $(SRC_DIR) -type d)

I_INC = $(addprefix -I, $(INCS))

CC = gcc
CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes $(I_INC)
LD = gcc
LDFLAGS = -lm

# ======= Test ======= #

TEST_DIR = test

TESTS = $(shell find $(TEST_DIR) -type f -name "*.c")
TEST_OBJS = $(TESTS:%.c=$(OBJ_DIR)/%.o)
TEST_INCS = $(shell find $(TEST_DIR) -type d)

TEST_I_INC = $(addprefix -I, $(TEST_INCS))

TEST_CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes $(I_INC) $(TEST_I_INC)

# ======= Rules ======= #

all:
	@make test_array
	@make test_list

# ======= Test Rules ======= #

test_array: course_tableau_test

test_list: course_liste_test

course_tableau_test: $(OBJ_DIR)/$(TEST_DIR)/course_tableau_test.o $(OBJ_DIR)/$(SRC_DIR)/course/course_tableau.o $(OBJ_DIR)/$(SRC_DIR)/escale/escale.o $(OBJ_DIR)/$(TEST_DIR)/seatest/seatest.o
	$(LD) $^ -o $@ $(LDFLAGS)

course_liste_test: $(OBJ_DIR)/$(TEST_DIR)/course_liste_test.o $(OBJ_DIR)/$(SRC_DIR)/course/course_liste.o $(OBJ_DIR)/$(SRC_DIR)/escale/escale.o $(OBJ_DIR)/$(TEST_DIR)/seatest/seatest.o
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c $(INCS) $(TEST_INCS)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(TEST_CFLAGS)

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCS)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

doc:
	@doxygen Doxyfile

clean:
	@rm -rf $(OBJ_DIR) course_tableau_test course_liste_test

git:
	@git pull
	@git add .
	@git commit -m Update
	@git push

tar:
	@tar -czvf ../tad-groupe33.tar.gz ../code ../rapport

.PHONY: all test_array test_list clean git tar
