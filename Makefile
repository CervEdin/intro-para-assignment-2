CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread -g

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

4-0:
	$(CXX) $(CXXFLAGS) -O3 benchmark_example.cpp -o bin/bench

4-1:
	$(CXX) $(CXXFLAGS) -fsanitize=thread -O3 benchmark_example_CGL.cpp -o bin/bench-CGL

4-2:
	$(CXX) $(CXXFLAGS) -fsanitize=thread -O3 benchmark_example_FGL.cpp -o bin/bench-FGL

test: test-1 test-2 test-3 test-4-1

test-1: 1
	./bin/1-integration 1 1

test-2: 2
	./bin/2-sieve 1 10000000
	./bin/2-sieve 2 10000000
	./bin/2-sieve 4 10000000
	./bin/2-sieve 8 10000000
	./bin/2-sieve 16 10000000
	./bin/2-sieve 1 100000000
	./bin/2-sieve 2 100000000
	./bin/2-sieve 4 100000000
	./bin/2-sieve 8 100000000
	./bin/2-sieve 16 100000000

test-3: 3
	./bin/3-mutex

test-4-0: 4-0
	./bin/bench 1

test-4-1: 4-1
	./bin/bench-CGL 1
	./bin/bench-CGL 2
	./bin/bench-CGL 4
	./bin/bench-CGL 8
	./bin/bench-CGL 16

test-4-2: 4-2
	./bin/bench-FGL 1
	./bin/bench-FGL 2

debug-2: 2
	gdb --args ./bin/2-sieve 8 2000000000

clean:
	$(RM) *.o
	ls | grep -v "\." | xargs $(RM)
