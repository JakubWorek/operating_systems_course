PROG=main
LDLIBS=-lrt
CFLAGS=-g

main: main.c
	$(CC) $^ -o $@ $(CFLAGS)

test: ${PROG}
	@echo -e "\n\nTESTING\n\n"
	@./${PROG}

clean:
	rm ${PROG}

.PHONY: clean test
