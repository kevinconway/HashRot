CC = gcc
FILES = hashrot.c sha2/sha2.c
OUT_EXE = hashrot
BUFFER_SIZE = 4096

all:
	$(CC) $(FILES) -o $(OUT_EXE) -D BUFFER_SIZE=$(BUFFER_SIZE) -pthread -w
