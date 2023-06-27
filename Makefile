# TARGET NAME
TARGET := k12s

# DIRECTORIES
EXE_DIR := ../exe/
OBJ_DIR := ../obj/
SRC_DIR := ./

# SOURCE FILES
SRCS := $(wildcard *.c)

# OBJECT FILES
OBJS := $(SRCS:%.c=$(OBJ_DIR)%.o)

# HEADER FILES
HEADERS := myTCP.h

# COMPILER & OPTIONS
CC := gcc
CFLAGS := -g -Wall -Wextra -DDEBUG=1
	# -D: definition for '#if DEBUG=1'
LFLAGS := -lpthread
	# DYNAMIC LIBRARY

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(EXE_DIR)$(TARGET)

# LINK OBJECTS
$(EXE_DIR)$(TARGET): $(OBJS)
	mkdir -p $(EXE_DIR)
	$(CC) $(CFLAGS) -o $(EXE_DIR)$@ $(OBJS) $(LFLAGS)

# COMPILE SOURCES
$(OBJ_DIR)%.o: %.c $(HEADERS)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(<:%.c=$(OBJ_DIR)%.o) -c $<

# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean:
	rm -f ./*~
	rm -f $(OBJS)
#	rm -f $(EXE_DIR)$(TARGET)
