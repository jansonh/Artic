arctic: main.o crypto.o generate.o utility.o
	g++ -o artic main.o crypto.o generate.o utility.o
main.o: main.cpp crypto.h generate.h utility.h
	g++ -c main.cpp
crypto.o: crypto.cpp generate.h utility.h
	g++ -c crypto.cpp
generate.o: generate.cpp
	g++ -c generate.cpp
utility.o: utility.cpp
	g++ -c utility.cpp

