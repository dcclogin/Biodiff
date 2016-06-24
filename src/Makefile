biodiff: main.o auxiliary.o cdiff.o ndiff.o retrieval.o
	gcc -o biodiff main.o auxiliary.o cdiff.o ndiff.o retrieval.o

main.o: main.c main.h
	gcc -c main.c

auxiliary.o: auxiliary.c auxiliary.h
	gcc -c auxiliary.c

cdiff.o: cdiff.c cdiff.h
	gcc -c cdiff.c 

ndiff.o: ndiff.c ndiff.h
	gcc -c ndiff.c

retrieval.o: retrieval.c retrieval.h
	gcc -c retrieval.c

clean:
	rm biodiff main.o auxiliary.o cdiff.o ndiff.o retrieval.o 
