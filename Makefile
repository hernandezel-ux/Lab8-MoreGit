CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
OBJS = main.o vector.o storage.o interface.o

minimat: $(OBJS)
	$(CC) $(CFLAGS) -o minimat $(OBJS)

main.o: main.c interface.h
vector.o: vector.c vector.h
storage.o: storage.c storage.h vector.h
interface.o: interface.c interface.h storage.h vector.h

clean:
	rm -f $(OBJS) minimat
