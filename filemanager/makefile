CC=g++
CFLAGS=-c -Wall -Wextra -Wfatal-errors -pedantic -ggdb -std=c++0x -O3 
LDFLAGS=-larmadillo
SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)
	
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
	
