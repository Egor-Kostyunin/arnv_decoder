CC = gcc

arnv_decoder: include/*.h main.c src/*.c 
	$(CC) -o $@ main.c src/*.c -I ./include

clean:
	rm -f arnv_decoder
