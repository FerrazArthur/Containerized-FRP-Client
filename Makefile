# Directory structure
SRCDIR = ./src
OBJDIR = ./build

# Source files
SOURCES = $(shell find $(SRCDIR) -name "*.c")
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Compilation flags
CC = gcc
CFLAGS = -Wall -pedantic -Wextra
CLINKS = -lldap -llber -lssl -lcrypto

# Executable
EXECUTABLE = my-frpc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(CLINKS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(CLINKS)

$(BUILDDIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(EXECUTABLE)
