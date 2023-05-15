TARGET := k06c
SRC := $(TARGET).c
OBJ := $(TARGET).o
TARGET_SV := k06s
SRC_SV := $(TARGET_SV).c
OBJ_SV := $(TARGET_SV).o

CFLAGS := -g -Wall
LFLAGS := -lm
CC := gcc $(CFLAG) $(LFLAGS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o ../$@ ../$^ $(LFLAGS)

server: $(OBJ_SV)
	$(CC) -o ../$(TARGET_SV) ../$^ $(LFLAGS)

$(OBJ): $(SRC)
	$(CC) -o ../$(OBJ) -c $^

clean:
	rm -f ../$(TARGET) ../$(OBJ)

