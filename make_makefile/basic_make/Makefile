main.o: main.c vector.h angle_related.h extern_variable.h
	gcc -c main.c -o main.o

vector.o: vector.c vector.h
	gcc -c vector.c -o vector.o

angle.o: angle.c angle_related.h
	gcc -c angle.c -o angle.o

output: main.o vector.o angle.o
	gcc main.o vector.o angle.o -o output
