main:main.c libcal.so
	gcc $^ -o $@

libcal.so :add.c sub.c
	gcc add.c sub.c -shared -fPIC -o libcal.so

.PHONY:clean
clean:
	rm main
