CC = gcc
BUILD_DIR = build
CFLAGS = -Wall -std=c11
TIME := /usr/bin/time

.PHONY: all tests clean

all: reverse_bytes reverse_block

reverse_bytes: reverse_bytes.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

reverse_block: reverse_block.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

tests:
	@ echo "Reverse bytes" >> pomiar_zad_02.txt
	@ echo "Small file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_bytes input_sm.txt output.txt 2>&1` >> pomiar_zad_02.txt
	@ echo "Medium file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_bytes input_mid.txt output.txt 2>&1` >> pomiar_zad_02.txt
	@ echo "Large file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_bytes input_lg.txt output.txt 2>&1` >> pomiar_zad_02.txt
	@ echo "Reverse block" >> pomiar_zad_02.txt
	@ echo "Small file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_block input_sm.txt output.txt 2>&1` >> pomiar_zad_02.txt
	@ echo "Medium file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_block input_mid.txt output.txt 2>&1` >> pomiar_zad_02.txt
	@ echo "Large file time: " `$(TIME) -f '%e' ./${BUILD_DIR}/reverse_block input_lg.txt output.txt 2>&1` >> pomiar_zad_02.txt

clean:
	rm -rf $(BUILD_DIR)