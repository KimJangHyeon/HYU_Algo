# Compiler and Compile options.
CC = gcc
SRCDIR = src/
INC = include/
LIBS = lib/

CFLAGS = -g -Wall -I $(INC)

# Macros specifying path for compile.
SRCS := src/project1-2.c src/LinkedMerge.c

OBJS := $(SRCS:.cpp=.o)

# Compile command.
TARGET = project1-2
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lpthread

# Delete binary & object files.
clean:
	rm $(TARGET)
