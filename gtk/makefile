CC=gcc
CFLAGS=-c -Wall -Wextra -Wfatal-errors -pedantic -ggdb
LDFLAGS=`pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS)
	
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
	
