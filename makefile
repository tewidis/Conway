# compile using gcc
CC = gcc

# compiler flags
# -g	adds debugging information to the executable file
# -Wall	turns on most compiler warnings
CFLAGS = -g -Wall

# the build target executable
TARGET = conway

# define the object files
OBJFILES = conway.o config_file.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	$(RM) $(TARGET) $(OBJFILES) *~
