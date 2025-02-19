all: build

build: main.c
	@echo "Building main.c"
	gcc main.c -o nurky -lncurses

clean:
	@if test -f nurky; then rm nurky; fi

locations: locations.c
	@echo "Building locations.c"
	gcc -c locations.c -o locations.o

app: app.c
	@echo "Building app.c"
	gcc -c app.c -o app.o

app_collector: app_collector.c
	@echo "Building app_collector.c"
	gcc -c app_collector.c -o app_collector.o

test: app locations app_collector
	@echo "building test file"
	gcc app.o locations.o app_collector.o -o test

autocomplete: autocomplete.c
	@echo "Building autocomplete.c"
	gcc -c autocomplete.c -lreadline -o autocomplete.o

input: app app_collector autocomplete input.c locations
	@echo "Building input.c"
	gcc input.c -lncurses app.o locations.o app_collector.o autocomplete.o -o input

launcher: launcher.c
	@echo "Building launcher.c"
	gcc -c launcher.c -o launcher.o

main: app app_collector autocomplete launcher main.c locations
	@echo "Building main.c"
	gcc main.c -lncurses app.o locations.o app_collector.o autocomplete.o launcher.o -o nurky

clean:
	@if test -f nurky; then rm nurky; fi
	@if test -f locations.o; then rm locations.o; fi
	@if test -f app.o; then rm app.o; fi
	@if test -f app_collector.o; then rm app_collector.o; fi
	@if test -f test; then rm test; fi