# Directory structure
SRCDIR = ./src
OBJDIR = ./build

# Source files
SOURCES = $(shell find $(SRCDIR) -name "*.c")
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Compilation flags
CC = gcc
CFLAGS = -Wall -pedantic

# Executable
EXECUTABLE = quant1_frpc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(EXECUTABLE)
