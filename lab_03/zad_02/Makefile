CC = gcc
BUILD_DIR = build
CFLAGS = -Wall

.PHONY: afile_size_reader tests clean

file_size_reader: file_size_reader.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

tests:
	@touch file1
	@touch file2
	@touch file3

	@echo "asdasfasfasfawrq3rsgegdf4gtw34gwt" > file1
	@echo "asdasfasfasfawrq3rsgegdf4gtw34gwt" > file2
	@echo "asdasfasfasfawrq3rsgegdf4gtw34gwt" > file3

	@echo "Program output: "
	@./$(BUILD_DIR)/file_size_reader
	@echo "wc --bytes output: "
	@wc --bytes *

clean:
	rm -rf $(BUILD_DIR)
	rm -f file1 file2 file3