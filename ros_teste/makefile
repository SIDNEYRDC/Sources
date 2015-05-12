################################################################################
# Robot Simulator Makefile, written by SIDNEY RDC, 2014
################################################################################
# Compiler definitions --------------------------------------------------------#
CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -L./ -Wl,-rpath=./
LIBS = -lros_interface
INCLUDES = -I./

# Main directories ------------------------------------------------------------#
OBJDIR = .
OUTDIR = .

# Main files ------------------------------------------------------------------#
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE = $(addprefix $(OUTDIR)/, main)

# Rules -----------------------------------------------------------------------#
all: $(SOURCES) $(EXECUTABLE)
	@echo All simulator sources are compiled!

$(EXECUTABLE): $(OBJECTS)
	$(CC) -s -o $@ $^ $(LIBS) $(LDFLAGS) $(INCLUDES)

$(OBJECTS): $(SOURCES)
	$(CC) -c -o $@ $(subst .o,.cpp,$(subst $(OBJDIR),.,$@)) $(CFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

