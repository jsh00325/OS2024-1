run: hw1
	./hw1.out

hw1: main.o fishing_rod.o duck.o
	gcc -o hw1.out main.o fishing_rod.o duck.o

main.o: main.c
	gcc -c main.c
	
fishing_rod.o: fishing_rod.c duck_hunt.h
	gcc -c fishing_rod.c

duck.o: duck.c duck_hunt.h
	gcc -c duck.c

clean: 
	/bin/rm -f core *.o *.out