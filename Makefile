bm: main.c func.c
	clang -Wall -Werror -Wpedantic main.c func.c -o bm

clean:
	rm bm
restart:
	rm ~/.cash/*.bin && \
	touch ~/.cash/kopta.bin ~/.cash/log.bin

