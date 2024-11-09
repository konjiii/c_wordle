CC = gcc
CFLAGS = -Wall -Wextra -g -O3

LIBS = -lncurses -lcurl

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/wordle

all: $(EXEC)

$(EXEC): $(OBJ_FILES) $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $(EXEC) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all clean
