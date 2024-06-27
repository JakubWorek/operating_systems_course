CC = gcc
BUILD_DIR = build
CFLAGS = -Wall

.PHONY: execute_ls tests clean

execute_ls: execute_ls.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

tests: 
	./$(BUILD_DIR)/execute_ls .

clean:
	rm -rf $(BUILD_DIR)