# compile using gcc
CC = gcc

# compiler flags
# -g	adds debugging information to the executable file
# -Wall	turns on most compiler warnings
CFLAGS = -g -Wall

# the build target executable
TARGET = conway
DEPEND = config_file

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(DEPEND).c

clean:
	$(RM) $(TARGET)
