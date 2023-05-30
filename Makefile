TARGET := k10c
OBJ := $(TARGET).o
SRC := $(OBJ:%.o=%.c)
TARGET_SV := k10s
OBJ_SV := $(TARGET_SV).o
SRC_SV := $(OBJ_SV:%.o=%.c)
HEADER := myTCP.h

# COMPILER & OPTIONS
CFLAGS := -g -Wall -Wextra -DDEBUG=1
#		-D: definition for '#if DEBUG=1'
LFLAGS := -lpthread
CC := gcc $(CFLAGS)

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(TARGET) $(TARGET_SV)

$(TARGET): $(OBJ)
	$(CC) -o ../$@ ../$^ $(LFLAGS)

$(TARGET_SV): $(OBJ_SV)
	$(CC) -o ../$(TARGET_SV) ../$^ $(LFLAGS)

$(OBJ): $(SRC) $(HEADER)
	$(CC) -o ../$(OBJ) -c $(SRC)

$(OBJ_SV): $(SRC_SV) $(HEADER)
	$(CC) -o ../$(OBJ_SV) -c $(SRC_SV)


# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean:
	rm -f ../$(TARGET) ../$(OBJ)
	rm -f ../$(TARGET_SV) ../$(OBJ_SV)
	rm -f ./*~
