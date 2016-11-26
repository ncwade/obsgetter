TARGET = obsgetter
LIBS = -lcurl -lxml2
CC = gcc
INCLUDE=./include
CFLAGS = -g -I$(INCLUDE)

# If compiling on OSX, we need to install libxml2 through brew.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/local/opt/libxml2/include/libxml2/
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard include/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -Wall $(LIBS) -o $@ $(OBJECTS)

clean:
	-rm -f src/*.o
	-rm -f $(TARGET)