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
	gcc autocomplete.c -lreadline -o autocomplete.o

clean:
	@if test -f nurky; then rm nurky; fi
	@if test -f locations.o; then rm locations.o; fi
	@if test -f app.o; then rm app.o; fi
	@if test -f app_collector.o; then rm app_collector.o; fi
	@if test -f test; then rm test; fi