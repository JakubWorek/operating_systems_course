CC=gcc
BUILD_DIR=build
CFLAGS := -Wall
TIME := /usr/bin/time

.PHONY: all clean


all: integral_calculator integral_manager

integral_manager: integral_manager.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

integral_calculator: integral_calculator.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

run_calculator: integral_calculator 
	./$(BUILD_DIR)/integral_calculator

run_manager: integral_manager
	./$(BUILD_DIR)/integral_manager

clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.fifo