all: sortTester

sortTester : main.o BucketSort.o
	g++ -std=c++14 -Wall -Werror -O2 -pthread -o sortTester main.o BucketSort.o

main.o: main.cpp BucketSort.h
	g++ -std=c++14 -Wall -Werror -O2 -pthread -c main.cpp

BucketSort.o : BucketSort.h BucketSort.cpp
	g++ -std=c++14 -Wall -Werror -O2 -pthread -c BucketSort.cpp

clean:
	rm *.o sortTester