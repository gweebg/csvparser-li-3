CC = gcc
CFLAGS  = -g -std=gnu99 -Wall -Wextra -pthread $(shell pkg-config --cflags glib-2.0)
LDFLAGS = $(shell pkg-config --libs glib-2.0) -lm 
OPTIM_FLAGS = -DNDEBUG -O2 -march=native -mtune=native -flto
CFLAGS += $(OPTIM_FLAGS)

SRC_DIR = src
INC_DIR = include
BIN_DIR = obj
EXECUTABLE_NAME = guiao-1.out
EXECUTABLE_NAME_SYMLINK = prog

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))
DEP = $(OBJ:%.o=%.d)

$(EXECUTABLE_NAME): $(BIN_DIR)/$(EXECUTABLE_NAME)

$(BIN_DIR)/$(EXECUTABLE_NAME): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	ln -sf $@ $(EXECUTABLE_NAME_SYMLINK)
	mkdir -p entrada saida

-include $(DEP)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_DIR) -MMD -c $< -o $@

.PHONY: clean
clean:
	-rm -rf obj/* $(EXECUTABLE_NAME_SYMLINK)
	-rm -rf build
	-rm saida/*
