# Compiler and Compilation Flags
CC = gcc
CFLAGS = -Wall -pedantic
LDFLAGS =

# Directories
SRCDIR = src/c_version
OBJDIR = $(SRCDIR)/build
BINDIR = .

# Source files
SOURCES := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)

# Object files
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Target Executable
TARGET = $(BINDIR)/quant1_frpc

# Build Rule
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Object File Compilation Rule
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean Rule
clean:
	rm -f $(OBJFILES) $(TARGET)

.PHONY: clean

# Build and Run Rule
run: $(TARGET)
	./$(TARGET)

# Default Target
all: $(TARGET)

.PHONY: run all

