CC=gcc
BUILD_DIR=build
CFLAGS= -Wall

.PHONY: all client_static client_shared_so client_shared_dlopen clean

all:
	make client_static
	make client_shared_so
	make client_shared_dlopen

client_static: client_static.c
	mkdir -p $(BUILD_DIR)
	make -C collatzlib collatz_static
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< collatzlib/build/libcollatz_static.a

client_shared_so: client_shared_so.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib
	$(CC) $(CFLAGS) -L ./collatzlib/build $< -lcollatz_shared -Wl,-rpath=./collatzlib/build -o $(BUILD_DIR)/$@ 

client_shared_dlopen: client_shared_dlopen.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -ldl

test_static:
	./$(BUILD_DIR)/client_static

test_shared_so:
	./$(BUILD_DIR)/client_shared_so

test_shared_dlopen:
	./$(BUILD_DIR)/client_shared_dlopen

clean:
	rm -rf $(BUILD_DIR)
	make -C collatzlib clean