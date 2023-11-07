# Define compiler and compiler flags
CC = gcc
CFLAGS = -Wall -pedantic

# Directories
SRCDIR = ./src/c_version
OBJDIR = $(SRCDIR)/build/objects

# Source files
SOURCES = $(wildcard $(SRCDIR)/*/*.c)

# Object files
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Final executable
EXECUTABLE = quant1_frpc

# Default target
all: $(EXECUTABLE)

# Link the object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
