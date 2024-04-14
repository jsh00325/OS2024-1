MakeTest: main.o hello.o world.o
	gcc -o MakeTest.out main.o hello.o world.o

clean: 
	/bin/rm -f core *.o

main.o: main.c
	gcc -c main.c

hello.o: hello.c
	gcc -c hello.c

world.o: world.c
	gcc -c world.c