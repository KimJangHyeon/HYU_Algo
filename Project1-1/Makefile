# Compiler and Compile options.
CC = gcc
CFLAGS = -g -Wall

# Macros specifying path for compile.
SRCS := project1-1.c
OBJS := $(SRCS:.cpp=.o)

# Compile command.
TARGET = project1-1
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Delete binary & object files.
clean:
	rm $(TARGET) $(OBJS)
