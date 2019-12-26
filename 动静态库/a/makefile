main:main.c libcal.a
	gcc $^ -o $@
libcal.a:add.c sub.c
	gcc -c sub.c -o sub.o
	gcc -c add.c -o add.o   
	ar -rc libcal.a add.o sub.o


.PHONY:clean
clean:
	rm main

