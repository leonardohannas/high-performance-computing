CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

SEQ_TARGET = studentsseq
PAR_TARGET = studentspar

SEQ_SRC = studentsseq.c
PAR_SRC = studentspar.c
STATS_SRC = stats.c

STATS_OBJ = stats.o
PAR_OBJ = studentspar.o

.PHONY: all build run clean

all: build

build: $(SEQ_TARGET) $(PAR_TARGET)

$(SEQ_TARGET): $(SEQ_SRC)
	$(CC) $(CFLAGS) -o $(SEQ_TARGET) $(SEQ_SRC)

$(PAR_TARGET): $(PAR_OBJ) $(STATS_OBJ)
	$(CC) $(CFLAGS) -o $(PAR_TARGET) $(PAR_OBJ) $(STATS_OBJ) $(LDFLAGS)

$(PAR_OBJ): $(PAR_SRC) stats.h
	$(CC) $(CFLAGS) -c $(PAR_SRC)

$(STATS_OBJ): $(STATS_SRC) stats.h
	$(CC) $(CFLAGS) -c $(STATS_SRC)

run: run-seq

run-seq: $(SEQ_TARGET)
	./$(SEQ_TARGET)

run-par: $(PAR_TARGET)
	./$(PAR_TARGET)

clean:
	rm -f $(SEQ_TARGET) $(PAR_TARGET) $(PAR_OBJ) $(STATS_OBJ)

.DEFAULT_GOAL := all
