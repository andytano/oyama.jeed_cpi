TARGET := ${arg}
SRC := $(TARGET).cpp
OBJ := $(TARGET).o

CFLAGS := -g -Wall
LFLAGS := -lm -pthread
OPTION := `pkg-config --cflags --libs opencv4`
CC := g++ 

all : $(TARGET)

$(TARGET): $(SRC) 
	$(CC) $^ $(OPTION) -o $@
clean:
	rm -f $(TARGET) $(OBJ)
