CC=gcc -Wall
SRC2=main.c
OUT=out/main
LIB=json
LIB2=crypto
all:$(SRC2)
	$(CC) $(SRC2) -l $(LIB) -o $(OUT) -l $(LIB2)