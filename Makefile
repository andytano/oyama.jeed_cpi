# TARGET NAME
TARGET := k12c
SRC := $(OBJ:%.o=%.c)
TARGET_SV := k12s
SRC_SV := $(OBJ_SV:%.o=%.c)
HEADER := myTCP.h

# OBJECT FILES
OBJ := $(TARGET).o
OBJS := $(TARGET).o clThread_12.o tcpip_12.o utils_12.o
OBJ_SV := $(TARGET_SV).o


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
	
------------------------------------------------	
# SOURCE FILES
SRCS := $(OBJS:%.o=%.c)

# COMPILER TYPE
CC := gcc

# COMPILER OPTION
CFLAGS := -g -Wall -Wextra -DDEBUG=1
#         -D: definition for '#if DEBUG=1'

# DYNAMIC LIBRARY
LDFLAGS := -lpthread

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(TARGET)

# LINK OBJECTS
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# COMPILE SOURCES
./%.o: ./%.c
	$(CC) $(CFLAGS) -c $<



# UPDATE HEADER FILE
$(OBJS): myTCPSv.h

# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean:
	rm -f *.o *~ $(TARGET)

