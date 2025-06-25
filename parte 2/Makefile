CC=gcc
CFLAGS=-I.
DEPS = tp.h bmh.h huffman.h
OBJ = main.o tp.o bmh.o huffman.o
EXEC = tp3_parte2

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core