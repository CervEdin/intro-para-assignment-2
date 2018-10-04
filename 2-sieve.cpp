// For this exercise, you will use Posix threads to implement a parallel version of the Sieve of Eratosthenes, an algorithm to find prime numbers. For some maximum positive integer M ax, the Sieve of Eratosthenes works as follows:

/*

2. Set k to 2, the first unmarked number in the list.
3. 	repeat
		Mark all multiples of k between k^2 and Max.
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
#include <cmath>
#include <vector>

void sequential_sieve(std::vector<int> &natural_numbers, int k, int max)
{
	int k_squared = k*k;
	// repeat until k^2 is greater than max
	while (k_squared <= max) {
		// Mark all multiples of k between k^2 and Max
		for (int i = k_squared - 1; i < max; i += k)
		{
			natural_numbers[i] = 0;
		}
		// Find the smallest number greater than k that is still unmarked.
		while (natural_numbers[k] == 0) {
			k++;
		}
		k = natural_numbers[k];
		k_squared = k*k;
	}
}

void mark(std::vector<int> &unmarked, int lower_sqrt_max, int start, int end)
{
	// Find the next k
	int k = 1;
	int k_squared = 1;

	while (k <= lower_sqrt_max)  {

		// Find the next prime in the range 1..sqrt(max)
		while (unmarked[k-1] == 0) {
			k++;
		}
		k_squared = k*k;

		// Find the first multiple of k in the range start..end
		int first_multiple = ((start + 1) <= k) ? (2*k) - 1 : start;
		while ((first_multiple + 1) % k && first_multiple <= end) {
			first_multiple++;
		}
		
		for (int i = first_multiple; i <= end ; i += k)
		{
			unmarked[i] = 0;
		}
		k++;
	}
}

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
	
	int threads;
	try
	{
		threads = std::stoi(argv[1]);
	}
	catch (std::exception)
	{
		usage(argv[0]);
	}

	int max;
	try
	{
		max = std::stoi(argv[2]);
	}
	catch (std::exception)
	{
		usage(argv[0]);
	}

	// 1. Create a list of natural numbers: 1, 2, 3, ... , Max.
	//const int MAX = max;
	std::vector<int> natural_numbers;
	natural_numbers.resize(max);
	for (int i = 0, j = 1; i < max; i++, j++) 
	{
		natural_numbers[i] = j;
	}

	// Mark 1 as not prime
	natural_numbers[0] = 0;
	auto start_time;
	std::chrono::duration<double> duration

	if (threads == 1) {
		sequential_sieve(natural_numbers, 2, max);
	} 
	else 
	{
		double sqrt_max = sqrt(max);
		int lower_sqrt_max = (int)sqrt_max;
		// 1. First sequentially compute primes up to √Max.
		sequential_sieve(natural_numbers, 2, lower_sqrt_max);

		int nr_not_marked = max - lower_sqrt_max;
		int chunk_size = nr_not_marked / threads;
		// Make chunks even
		// chunk_size = chunk_size + (chunk_size % 2);
		int remainder = nr_not_marked - (chunk_size * threads);

		// This part of the array is processed
		int start = 0;
		int end = lower_sqrt_max - 1;

		std::thread *sieves = new std::thread[threads];
		start_time = std::chrono::system_clock::now()
		for (int thread = 0; thread < threads; thread++)
		{
			start = end + 1;
			end = start + chunk_size - 1;
			end = (thread + 1 == threads && remainder != 0) ? 
				end + remainder:
				end;
			sieves[thread] = std::thread(mark, std::ref(natural_numbers), lower_sqrt_max, start, end);
		}
		
		for (int thread = 0; thread < threads; thread++)
		{
			sieves[thread].join();
		}
		duration = std::chrono::system_clock::now() - start_time;
	}

	int nr_primes = 0;
	for (int i = 0; i < max; i++)
	{
		nr_primes += natural_numbers[i] ? 1 : 0;
	}

    //std::cout << "There were " << nr_primes << " primes." << std::endl;
    std::cout << " & " << threads << " threads." << std::endl;
    std::cout << " & " << duration.count()*1000 << " ms." << std::endl;

	return 0;
}
