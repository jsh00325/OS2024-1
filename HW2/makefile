run: myshell
	./myshell

myshell: my_shell.c my_shell.h get_token.o execute.o mybin/add mybin/mul
	gcc -o myshell my_shell.c get_token.o execute.o

get_token.o: get_token.c my_shell.h
	gcc -c get_token.c

execute.o: execute.c my_shell.h
	gcc -c execute.c

mybin/add: mybin/add.c
	cd mybin/ && gcc -o add add.c

mybin/mul: mybin/mul.c
	cd mybin/ && gcc -o mul mul.c

clean: 
	/bin/rm -f core *.o *.out