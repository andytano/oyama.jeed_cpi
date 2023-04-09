TARGET := thread1
SRC := $(TARGET).c
OBJ := $(TARGET).o

CFLAGS := -g -Wall
LFLAGS := -lm -pthread
CC := gcc $(CFLAG) $(LFLAGS)

all : $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) -o $@ $^ $(LFLAGS)

.c.o:
	$(CC) -c $^

clean:
	rm -f $(TARGET) $(OBJ)

