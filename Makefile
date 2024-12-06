all: build run

build: main.c
	@echo "Building main.c"
	gcc main.c -o nurky -lncurses

clean:
	@if test -f nurky; then rm nurky; fi

run: nurky
	./nurky firefox