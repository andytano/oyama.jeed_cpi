TARGET := k07c
SRC := $(TARGET).c
OBJ := $(TARGET).o
TARGET_SV := k07s
SRC_SV := $(TARGET_SV).c
OBJ_SV := $(TARGET_SV).o

CFLAGS := -g -Wall
LFLAGS := -lm
CC := gcc $(CFLAG) $(LFLAGS)

all: $(TARGET) $(TARGET_SV)

$(TARGET): $(OBJ)
	$(CC) -o ../$@ ../$^ $(LFLAGS)

$(TARGET_SV): $(OBJ_SV)
	$(CC) -o ../$(TARGET_SV) ../$^ $(LFLAGS)

$(OBJ): $(SRC)
	$(CC) -o ../$(OBJ) -c $^

$(OBJ_SV): $(SRC_SV)
	$(CC) -o ../$(OBJ_SV) -c $^

clean:
	rm -f ../$(TARGET) ../$(OBJ)
	rm -f ../$(TARGET_SV) ../$(OBJ_SV)
	rm -f ./*~
