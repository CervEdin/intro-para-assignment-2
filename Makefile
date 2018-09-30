CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread

all: test

1:
	$(CXX) $(CXXFLAGS) 1-integration.cpp -o 1-integration
	for N in 1 10 100 1000 10000 100000 1000000; do \
		for T in 1 2 4 8 16; do \
			echo "\n t=$$T" ; \
			./1-integration $$T $$N ; \
		done \
	done
2:
	$(CXX) $(CXXFLAGS) 2-sieve.cpp -o bin/2-sieve

3:
	$(CXX) $(CXXFLAGS) 3-mutex.cpp -o bin/3-mutex

4:
	$(CXX) $(CXXFLAGS) -O3 benchmark_example.cpp -o bin/bench

test: test-1 test-2 test-3 test-4

test-1: 1
	./bin/1-integration 1 1

test-2: 2
	./bin/2-sieve 1 10

test-3: 3
	./bin/3-mutex

test-4: 4
	./bin/bench 1

crash: 4
	./bin/bench 2

clean:
	$(RM) *.o
	ls | grep -v "\." | xargs $(RM)
