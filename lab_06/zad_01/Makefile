CC=gcc
BUILD_DIR=build
CFLAGS := -Wall
TIME := /usr/bin/time

.PHONY: all clean

all: integral

integral: integral.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -lm

test: integral
	@echo "20 processes	" `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 20 2>&1` "[s]"
	@echo "10 processes " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 10 2>&1` "[s]"
	@echo "5  processes " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 5  2>&1`  "[s]"
	@echo "1  process   " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 1  2>&1`  "[s]"
	
clean:
	rm -rf $(BUILD_DIR)