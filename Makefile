all: get-deps test prog clean

default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get update -y
	sudo apt-get  install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall  --std=c99 -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall --std=c99 -c spell.c

test.o: test_main.c
	gcc -Wall --std=c99 -c tests/test_main.c

main.o: main.c
	gcc -Wall --std=c99 -c main.c

test: dictionary.o spell.o test_main.o
	gcc -Wall  --std=c99 -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

prog: dictionary.o spell.o main.o
	gcc -Wall  --std=c99 -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o test_main dictionary.h.gch

cleanall:clean
	rm spell_check

	
