INCDIR=-I/home/muggli/git/cosmo/3rd_party_inst/include -I../rapidcheck/include
LIBDIR=-L../rapidcheck  -L/home/muggli/git/cosmo/3rd_party_inst/lib 

all: LowReader.o HighReader.o SDIter.o test


test: test.cpp LowReader.o HighReader.o SDIter.o
	g++ -std=c++11 -march=native -Wall -Wextra -DNDEBUG -g $(INCDIR) $(LIBDIR) test.cpp LowReader.o HighReader.o SDIter.o  -lsdsl  -lrapidcheck -o test


LowReader.o: LowReader.cpp LowReader.h
	g++ -g LowReader.cpp -c -o LowReader.o

HighReader.o: HighReader.cpp HighReader.h
	g++ -g  HighReader.cpp    -c -o HighReader.o

SDIter.o: SDIter.cpp SDIter.h
	g++ -g  SDIter.cpp    -c -o SDIter.o

clean:
	rm LowReader.o HighReader.o SDIter.o test

#example: example.cpp
#	g++ -std=c++14 -L../rapidcheck -I../rapidcheck/include  example.cpp -lrapidcheck -o example
