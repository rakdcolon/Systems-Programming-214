CC = gcc
CFLAGS = -fsanitize=address -Wall -g

SRC = mymalloc.c memgrind.c

TARGET = memgrind

$(TARGET): 
	$(CC) -o $@ $(SRC) $(CFLAGS)

clean:
	rm -f $(TARGET)
	rm -r $(TARGET).dSYM