OPTIMIZE = -O2

CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randmain: randmain.c randcpuid.c randlibsw.so randlibhw.so
	$(CC) $(CFLAGS) -Wl,-rpath=$(PWD) randmain.c randcpuid.c -ldl -o randmain 

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -shared -fPIC randlibsw.c -o randlibsw.so

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -shared -fPIC randlibhw.c -o randlibhw.so