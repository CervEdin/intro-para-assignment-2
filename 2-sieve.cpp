// For this exercise, you will use Posix threads to implement a parallel version of the Sieve of Eratosthenes, an algorithm to find prime numbers. For some maximum positive integer M ax, the Sieve of Eratosthenes works as follows:

/*
1. Create a list of natural numbers: 1, 2, 3, ... , Max.
2. Set k to 2, the first unmarked number in the list.
3. 	repeat
		Mark all multiples of k between k 2 and Max.
		Find the smallest number greater than k that is still unmarked.
		Set k to this new value.
	until k^2 is greater than Max.
4. The unmarked numbers are all prime.
*/


// To parallelize this algorithm,
// 1. First sequentially compute primes up to √Max.

// 2. Given p cores, build p chunks of roughly equal length covering the range from √Max + 1 to Max and allocate a thread for each chunk.

// 3. Each thread uses the sequentially computed “seeds” to mark the numbers in its chunk.

// 4. The master waits for all threads to finish and collects the unmarked numbers.

// Besides your code, you need to provide a brief report that explains your solution (e.g., did you use synchronization between threads and why, how did you distribute the work to threads, how did you minimize communication, how did you achieve load balance, etc.) and reports the speedup curve you get as the number of cores is increased. You should also try to vary the size of M ax to gauge the impact of program size on parallel performance. Make sure to pick large numbers for M ax (e.g., in the millions to ensure there’s enough work for threads to do).

#include <iostream>
#include <thread>

void usage(char *program)
{
	std::cout << "Usage: " << program << " T M" << std::endl;
	std::cout << std::endl;
	std::cout << "  T: number of threads" << std::endl;
	std::cout << "  M: Upper bound" << std::endl;
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		usage(argv[0]);
	}
}
