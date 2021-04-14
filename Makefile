all: main clear
main: main.o all.o databasein.o tablein.o
	g++ main.o all.o databasein.o tablein.o -o $@
main.o: main.cpp all.h databasein.h getstring.h skipspecialchar.h tablein.h vague_equal.h
	g++ -c main.cpp -o $@
all.o: all.h databasein.h all.cpp
	g++ -c all.cpp -o $@
databasein.o: databasein.h databasein.cpp tablein.h getstring.h vague_equal.h
	g++ -c databasein.cpp -o $@
tablein.o: tablein.cpp tablein.h skipspecialchar.h getstring.h vague_equal.h
	g++ -c $< -o $@
.PHONY: clear all
clear:
	rm *.o