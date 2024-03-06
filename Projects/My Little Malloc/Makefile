CC = gcc
CFLAGS = -fsanitize=address -Wall -g

SRC = mymalloc.c memgrind.c

TARGET = memgrind

all: $(TARGET)

$(TARGET): 
	$(CC) -o $@ $(SRC) $(CFLAGS)
