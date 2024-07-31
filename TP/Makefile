CC = gcc
CFLAGS = -I -Wall --pedantic -std=gnu17
DEPS = writer.h common.h reader.h
OBJ1 = writer.o common.o
OBJ2 = reader.o common.o
ODIR=.



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: writer reader

writer: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS)

reader: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~