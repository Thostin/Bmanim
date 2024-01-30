bm: main.c lfunc.c
	clang -Wall -Werror -Wpedantic func.c main.c lfunc.c -o bm

clean:
	rm bm
	rm ~/storage/shared/xd.bmp 

