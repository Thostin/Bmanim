bm: main.c func.c
	clang -Wall -Werror -Wpedantic main.c func.c -o bm

clean:
	rm bm
	rm ~/storage/shared/xd.bmp 

