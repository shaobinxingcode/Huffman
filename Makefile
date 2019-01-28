CC = g++
huffman: huffman.o
	$(CC) -o huffman huffman.o
main.o : huffman.c global.h encode.h decode.h search.h
	$(CC) -c huffman.c	

clean : 
	; rm huffman ; rm huffman.o
