# Makefile for compiling the C project

# Compiler and compiler flags
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -g

# Source files
SRCS = traceroute.c checking_util.c printing_util.c sending_util.c waiting_util.c

# Object files (replace .c extensions with .o)
OBJS = $(SRCS:.c=.o)

# Executable name
MAIN = traceroute

.PHONY: clean

# Default target
all: $(MAIN)
	@echo Compilation complete.

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning old files before new make
clean:
	$(RM) $(OBJS) $(MAIN) *~