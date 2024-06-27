CC = gcc
BUILD_DIR = build
CFLAGS = -Wall

.PHONY: spawn_proceses tests clean

spawn_procesess: spawn_procesess.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

tests: 
	./$(BUILD_DIR)/spawn_procesess 10

clean:
	rm -rf $(BUILD_DIR)