CC = gcc
INCLUDE_DIR = include
BINARIES = $(patsubst src/%.c,bin/%.o, $(wildcard src/*.c))
# Remove -g if not in debug mode
COMPILE_FLAGS = -g

.PHONY: all test clean
all: app.exe

run: app.exe
	./app.exe

app.exe: $(BINARIES)
	$(CC) $^ -o $@ -I $(INCLUDE_DIR)

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) -c -o $@ -I$(INCLUDE_DIR) $(COMPILE_FLAGS) $^

clean:
	rm -rf bin/ app.exe
