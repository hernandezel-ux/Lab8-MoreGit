CC=gcc
CFLAGS=-c -Wall -Wextra -pedantic -std=c11 -g
LDFLAGS=
SOURCES=main.c vector.c storage.c interface.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=minimat

all: $(EXECUTABLE)

-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d

.PHONY: all clean
