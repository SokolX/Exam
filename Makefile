CC=gcc -Wall
SRC2=main.c
OUT=out/main
all:$(SRC2)
	$(CC) $(SRC2) -l json -o $(OUT) 