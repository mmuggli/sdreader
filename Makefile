INCDIR=-I/home/muggli/git/cosmo/3rd_party_inst/include -I../rapidcheck/include
LIBDIR=-L../rapidcheck  -L/home/muggli/git/cosmo/3rd_party_inst/lib 
CPP_FLAGS=-std=c++11 -O2 -march=native
all: LowReader.o HighReader.o SDIter.o test


test: test.cpp LowReader.o HighReader.o SDIter.o
	g++ $(CPP_FLAGS) -Wall -Wextra -DNDEBUG  $(INCDIR) $(LIBDIR) test.cpp LowReader.o HighReader.o SDIter.o  -lsdsl  -lrapidcheck -o test


LowReader.o: LowReader.cpp LowReader.h
	g++ $(CPP_FLAGS) LowReader.cpp -c -o LowReader.o

HighReader.o: HighReader.cpp HighReader.h
	g++ $(CPP_FLAGS)  HighReader.cpp    -c -o HighReader.o

SDIter.o: SDIter.cpp SDIter.h
	g++ $(CPP_FLAGS)  SDIter.cpp    -c -o SDIter.o

clean:
	rm LowReader.o HighReader.o SDIter.o test

#example: example.cpp
#	g++ -std=c++14 -L../rapidcheck -I../rapidcheck/include  example.cpp -lrapidcheck -o example
