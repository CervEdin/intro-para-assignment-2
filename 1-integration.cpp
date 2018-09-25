// Numerical integration is a technique for numerical (approximate) calculation of a definite integral of a function f . The algorithm is defined as follows: The interval of the integral is partitioned into many small parts of size w. For each of those small inter- vals the area under the curve is approximated by the area of the trapeze defined by x j−1 , x j , f (x j−1 ) and f (x j ). Then the ap- proximation of the whole integral can be computed by summing up all trapeze areas. The total accuracy of the calculation can be scaled arbitrarily by increasing the total number of trapezes. As the calculation of one trapeze area is independent from the other areas, this parallel numerical integration algorithm can be parallelized quite easily.

// TASK a) Implement a parallel program in C11 or C++11 for numerical integration of the following function: Int(1,0)(4/(1+x^2)) dx
// Your program should accept the total number of threads and trapezes with appropriate command-line arguments. Also, the command line argument -h should print a help message and exit.

// TASK b) Test your program and measure the runtime (of the computation, without setup time) for various configurations (number of threads, number of trapezes, each starting at 1).
// Document your results in tabular form and analyze. By the way, did you notice something concerning the value of the computed integral?

// TASK c) Evaluate and document different methods of distributing the work among threads. Hint: you may consider using different numbers of threads and trapezes.

#include <iostream>
#include <thread>

void usage(char *program)
{
	std::cout << "Usage: " << program << " T Trapz" << std::endl;
	std::cout << std::endl;
	std::cout << "  H: Display help" << std::endl;
	std::cout << "  T: Nr of Threads" << std::endl;
	std::cout << "  Trapz: Nr of Trapezes" << std::endl;
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
//		std::cout << "Arg1 " << argv[1]
//		std::cout << "Arg2 " << argv[2]
		usage(argv[0]);
	}
}
