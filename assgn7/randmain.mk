randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain
randmain.o: randmain.c
	$(CC) $(CFLAGS) -c randmain.c
randcpuid.o: randcpuid.c
	$(CC) $(CFLAGS) -c randcpuid.c
randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared randlibsw.o -o randlibsw.so
randlibsw.o: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o randlibsw.o
randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) randlibhw.c -fPIC -shared -o randlibhw.so
randlibhw.o: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o randlibhw.o
