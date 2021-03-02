build: so-cpp.c
	cl /MD so-cpp.c -o so-cpp.exe

clean:
	del so-cpp.exe so-cpp.obj