CC=gcc -Wall
SRC2=main.c
OUT=out/main
OUT2=examserver
OUTPUTPATH=out/$(OUT)
all:$(SRC2)
	$(CC) $(SRC2) -o $(OUT) 