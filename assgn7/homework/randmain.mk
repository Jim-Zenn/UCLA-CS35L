OBJECTS = randmain.o randcpuid.o

randmain: $(OBJECTS)
	$(CC) $(CFLAGS) -o randmain $(OBJECTS) -static -ldl -Wl,-rpath=$PWD
randmain.o: randmain.c
	$(CC) $(CFLAGS) -c randmain.c
randcpuid.o: randcpuid.c
	$(CC) $(CFLAGS) -c randcpuid.c
randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -fPIC -shared -o randlibsw.so
randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -fPIC -shared -o randlibhw.so