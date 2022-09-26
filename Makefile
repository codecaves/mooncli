SRC_DIR = src
BUILD_DIR = build
CC = clang
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_NAME = mooncli
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
COMPILER_FLAGS = -std=c99 -Wall -g
LINKER_FLAGS = -lcurl -ljson-c

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	$(RM) -rf $(BUILD_DIR)/$(OBJ_NAME) $(BUILD_DIR)/*.dSYM