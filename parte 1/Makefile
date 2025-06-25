CC=gcc
CFLAGS=-I.
DEPS = tp.h shift_and.h dyn.h
OBJ = main.o shift_and.o dyn.o tp.o
EXEC = tp3_parte1

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core