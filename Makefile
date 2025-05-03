CC = gcc
CFLAGS = -I./src -fsanitize=address -g # for debugging
OBJS = ./test/main.o ./src/page.o
TARGET = page_test

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

all: $(TARGET)
	./page_test

clean:
	rm -f ./src/*.o ./test/*.o
	rm -f $(TARGET) pokemon_world.db
re:
	make clean
	make all
