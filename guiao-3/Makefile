# Compilador
CC = gcc

# Flags de compilação
CFLAGS   = -std=gnu99 -Wall -O2 $(shell pkg-config --cflags glib-2.0)
CFLAGS_T = -std=gnu99 -Wall -O2 $(shell pkg-config --cflags libcpuid) $(shell pkg-config --cflags glib-2.0)

# Flags de linking
LDFLAGS   = $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs libcpuid) -lm
LDFLAGS_T = -lm $(shell pkg-config --libs libcpuid) $(shell pkg-config --libs glib-2.0)

# Variáveis
SRC_DIR = src
INC_DIR = include
BIN_DIR = obj

SRC = $(shell find src -type f \( -iname "*.c" ! -iname "testing.c" \))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))
DEP = $(OBJ:%.o=%.d)

SRC_T = $(shell find src -type f \( -iname "*.c" ! -iname "main.c" \))
OBJ_T = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_T))
DEP_T = $(OBJ_T:%.o=%.d)

NAME   = prog
NAME_S = guiao-3

NAME_T   = tests
NAME_T_S = tests

# Programa principal
$(NAME): cleant
$(NAME): $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	ln -sf $@ $(NAME_S)
	mkdir -p entrada saida

-include $(DEP)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_DIR) -MMD -c $< -o $@

# Programa de testes
$(NAME_T): clean
$(NAME_T): $(BIN_DIR)/$(NAME_T)

$(BIN_DIR)/$(NAME_T): $(OBJ_T)
	mkdir -p $(@D)
	$(CC) $(CFLAGS_T) $^ $(LDFLAGS_T) -o $@
	ln -sf $@ $(NAME_T_S)

-include $(DEP)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS_T) -I$(INC_DIR) -MMD -c $< -o $@

.PHONY: clean
clean:
	-rm -rf obj/* $(NAME_S)
	-rm guiao-3
	-rm saida/*

.PHONY: cleant
cleant:
	-rm -rf obj/* $(NAME_T_S)
	-rm saida/*.txt