# TARGET NAME
TARGET := k12s
HEADER := myTCP.h

# OBJECT FILES
OBJ := $(TARGET).o clThread_12.o tcpip_12.o utils_12.o

# SOURCE FILES
SRC := $(OBJ:%.o=%.c)

# COMPILER & OPTIONS
CFLAGS := -g -Wall -Wextra -DDEBUG=1
#         -D: definition for '#if DEBUG=1'
LFLAGS := -lpthread
#         DYNAMIC LIBRARY
CC := gcc $(CFLAGS)

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(TARGET)

# LINK OBJECTS
$(TARGET): $(OBJ)
	$(CC) -o ../$@ ../$^ $(LFLAGS)

# COMPILE SOURCES
#./%.o: ./%.c
$(OBJ): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o ../$(OBJ) -c $(SRC)

# UPDATE HEADER FILE
$(OBJ): $(HEADER)

# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean:
	rm -f ../$(TARGET) ../$(OBJ)
	rm -f ./*~

