TARGET := k11c
OBJ := $(TARGET).o
SRC := $(OBJ:%.o=%.cpp)
TARGET_SV := k11s
OBJ_SV := $(TARGET_SV).o
SRC_SV := $(OBJ_SV:%.o=%.cpp)
HEADER := usbcamcom.h cvui.h

# COMPILER & OPTIONS
#OPT := `pkg-config --cflags --libs opencv4`
CFLAGS := -pthread -g -Wall -DDEBUG=1 `pkg-config --cflags opencv4`
#		-Wextra: enable extra warning
#		-D: definition for '#if DEBUG=1'
LFLAGS := -lm -pthread `pkg-config --libs opencv4`
CC := g++

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(TARGET) $(TARGET_SV)

$(TARGET): $(OBJ)
	$(CC) -o ../$@ ../$^ $(LFLAGS)

$(TARGET_SV): $(OBJ_SV)
	$(CC) -o ../$(TARGET_SV) ../$^ $(LFLAGS)

$(OBJ): $(SRC) $(HEADER)
	$(CC) -o ../$(OBJ) -c $(SRC) $(CFLAGS)

$(OBJ_SV): $(SRC_SV) $(HEADER)
	$(CC) -o ../$(OBJ_SV) -c $(SRC_SV) $(CFLAGS)


# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean:
	rm -f ../$(TARGET) ../$(OBJ)
	rm -f ../$(TARGET_SV) ../$(OBJ_SV)
	rm -f ./*~
