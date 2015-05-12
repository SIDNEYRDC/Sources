# Compiler variables
CC = g++
CFLAGS =-Wall -Wextra
LDFLAGS =
LIBS =
INCLUDES =

# Main directories
OBJDIR = ./obj
OUTDIR = ./bin

# Main files
SOURCES = $(wildcard *.c)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))
EXECUTABLE = $(addprefix $(OUTDIR)/, teste)

# Rules
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -s -o $@ $^ $(LIBS) $(LDFLAGS) $(INCLUDES)

$(OBJECTS): $(SOURCES)
	$(CC) -c -o $@ $(subst .o,.c,$(subst obj,.,$@)) $(CFLAGS)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)