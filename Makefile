
cc=gcc
cflags=-Wall -Wextra -I./src/include

all:
	$(cc) $(cflags) -o bin/emulator src/*.c
clean:
	rm -rf bin/emulator


