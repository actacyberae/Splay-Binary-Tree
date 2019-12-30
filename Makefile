all: main

main-valgrind: main.o lib.o SBT.o
	$(CC) main.o lib.o SBT.o -o main
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./main
	rm -rf *.o
	
main: main.o lib.o SBT.o
	$(CC) main.o lib.o SBT.o -lm -o main
	rm -rf *.o
	
lib.o: ./libs/lib.c
	$(CC) ./libs/lib.c -c ./libs/lib.c
	
SBT.o: ./libs/SBT.c
	$(CC) ./libs/SBT.c -c ./libs/SBT.c
	
insertion_main.o: main.c
	$(CC) main.c -c main.c
	
clean:
	rm -rf *.o main
