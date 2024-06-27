CC=gcc
BUILD_DIR=build
CFLAGS= -Wall

.PHONY: bibl1_shared

bibl1_shared: bibl1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -o $(BUILD_DIR)/$@.o -c $<
	$(CC) -shared -o $(BUILD_DIR)/lib$@.so $(BUILD_DIR)/$@.o

client_shared_dlopen: zad1.c
	mkdir -p $(BUILD_DIR)
	make bibl1_shared -C .
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -ldl

clean:
	rm -rf $(BUILD_DIR)