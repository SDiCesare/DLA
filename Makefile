CC = gcc
CFLAGS = -Wall -I.

# List of source files
SERIAL_DIR = serial
UTILS_DIR = utils
SRC_FILES = $(wildcard $(SERIAL_DIR)/*.c) $(wildcard $(UTILS_DIR)/*.c)

# List of header files
INC_DIR = $(SERIAL_DIR) $(UTILS_DIR)
INC_FILES = $(foreach dir, $(INC_DIR), -I$(dir))

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
EXEC = my_program

# Rule to build the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(INC_FILES) $^ -o $@

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) $(INC_FILES) -c $< -o $@

# PHONY target to clean the project
.PHONY: clean
clean:
	rm -f $(OBJ_FILES) $(EXEC)
