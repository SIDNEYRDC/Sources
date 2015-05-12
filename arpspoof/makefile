################################################################################
# Robot Simulator Makefile, written by SIDNEY RDC, 2014
# Last Change: 2015 Abr 18 00:38:30
################################################################################
# Compiler definitions --------------------------------------------------------#
CC = gcc
CFLAGS = -Wall -Wextra -Wfatal-errors -pedantic -ggdb3 -O0
LDFLAGS = -L./
LIBS =
INCLUDES = -I./

# Main files ------------------------------------------------------------------#
SOURCES = arpspoof.c
OBJECTS = arpspoof.o
EXECUTABLE = arpspoof

# Rules -----------------------------------------------------------------------#
# $@ = term before :
# $^ = all terms after :
# $< = first term after :
all: $(SOURCES) $(EXECUTABLE)
	@echo All sources are compiled!

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) $(INCLUDES)

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

