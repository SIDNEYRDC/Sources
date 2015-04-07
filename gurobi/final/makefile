CC=g++
CFLAGS=-c -Wall -Wextra -pedantic -ggdb -O2
LDFLAGS=-L/opt/gurobi563/linux64/lib -I/opt/gurobi563/linux64/include -lgurobi_g++4.1 -lgurobi56 -larmadillo
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
	
