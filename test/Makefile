all:
	cc --share -fPIC module.c -o libmodule.so
	cc --share -fPIC a.c -lmodule -L. -o a.so
	cc mylib.c -o test -ldl -lmodule -L. 

clean:
	-rm test *.so *.o
