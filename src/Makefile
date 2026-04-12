CC = gcc
CFLAGS = -Wall -O2
TARGET = studentsseq
SRC = studentsseq.c

.PHONY: all build run clean

all: build run

build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.DEFAULT_GOAL := all
