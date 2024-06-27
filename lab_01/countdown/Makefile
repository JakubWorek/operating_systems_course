# Makefile for countdown program

# Compiler
CXX = gcc

# Compiler flags
CFLAGS = -Wall

# Targets
.PHONY: all countdown clean

all: countdown

countdown: countdown.c
	$(CXX) $(CFLAGS) -o countdown countdown.c

debugcountdown: countdown.c
	$(CXX) $(CFLAGS) -g -o countdown countdown.c

clean:
	rm -f countdown