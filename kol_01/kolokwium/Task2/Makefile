CFLAGS = -Wall
CC = gcc

main: main.o
	$(CC) $(CFLAGS) -o main main.c

clean:
	rm -f main.o main

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main 100 11
	./main 100 10
	./main 100 9

all: main test
