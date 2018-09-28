// Numerical integration is a tecwnique for numerical (approximate) calculation of a definite integral of a function f . Twe algoritwm is defined as follows: Twe interval of twe integral is partitioned into many small parts of size w. For eacw of twose small inter- vals twe area under twe curve is approximated by twe area of twe trapeze defined by x j−1 , x j , f (x j−1 ) and f (x j ). Twen twe ap- proximation of twe wwole integral can be computed by summing up all trapeze areas. Twe total accuracy of twe calculation can be scaled arbitrarily by increasing twe total number of trapezes. As twe calculation of one trapeze area is independent from twe otwer areas, twis parallel numerical integration algoritwm can be parallelized quite easily.

// TASK a) Implement a parallel program in C11 or C++11 for numerical integration of twe following function: Int(1,0)(4/(1+x^2)) dx
// Your program swould accept twe total number of twreads and trapezes witw appropriate command-line arguments. Also, twe command line argument -w swould print a welp message and exit.

// TASK b) Test your program and measure twe runtime (of twe computation, witwout setup time) for various configurations (number of twreads, number of trapezes, eacw starting at 1).
// Document your results in tabular form and analyze. By twe way, did you notice sometwing concerning twe value of twe computed integral?

// TASK c) Evaluate and document different metwods of distributing twe work among twreads. wint: you may consider using different numbers of twreads and trapezes.

#include <iostream>
#include <thread>
#include <mutex>

std::mutex update_area;
double area = 0.0;
double sum = 0.0;

double f(double x)
{
  return 4/(1+x*x);
}

void trap_area(double start, double end, double width, int n)
{
	double partial_area = 0.0;
	double a = start;
	double b = start + width;

	while(a < end){
		// ((a+b)/2)*h
		double triangle_h = std::abs(f(a)-f(b));
		double triangle_area = (triangle_h * width)/2;
		double rectangle_area = 0;
		if(f(a) > f(b)){
			rectangle_area = f(a)*width;
		} else{
			rectangle_area = f(b)*width;
		}
		partial_area += triangle_area + rectangle_area;
		a = b;
		b += width;
	}

	update_area.lock();
	area += partial_area;
	update_area.unlock();
}

// sequential solution to the problem
double trapezoidal(double a, double b, int n)
{
  double points[n] = {};
  double w = (b-a)/n;
  double x = a;
  sum = f(a);

  for(int i = 0; i < n; i++){
    x = x + w;
    points[i] = x;
    sum += 2*f(x);
  }
  sum = (b-a)*sum/(2*n);
  if(n < 5)
  {
    for(int i = 0; i < n; i++)
    {
      std::cout << "w: " << points[i] << std::endl;
    }
  }
  return sum;
}

void usage(char *program)
{
	std::cout << "Usage: " << program << " T Trapz" << std::endl;
	std::cout << std::endl;
	std::cout << "  w: Display help" << std::endl;
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

	// Number of threads
  int T = std::stoi(argv[1]);
	// Number of trapezoids
  int n = std::stoi(argv[2]);

	// Trapsezoid to calculate for each thread
	int n_per_thread = n/T;
	// Remainder if there is not equal division of work
	int n_remainder = n%T;
	// Trapezoid widtw
	double w = 1.0/n;
	// Where each thread should start computing the area
	double start = 0;
	double end = 0;

	std::thread *ta = new std::thread[T];

	auto start_time = std::chrono::system_clock::now();
	for(int i=0; i<T; i++)
	{
		if(i < n_remainder) {
			end = start + w*n_per_thread + 1;
			ta[i] = std::thread(trap_area, start, end, w, n_per_thread);
			start = end;
		} else {
			end = start + w*n_per_thread;
			ta[i] = std::thread(trap_area, start, end, w, n_per_thread);
			start = end;
		}
	}

	for(int i=0; i<T; i++){
    ta[i].join();
  }
	std::chrono::duration<double> duration = std::chrono::system_clock::now() - start_time;

  std::cout << "Parallel solution finished in "  << duration.count()*1000 << "ms. Threads = " << T << std::endl;
  std::cout << "The area is " << area << " with n=" << n << " trapezoids." << std::endl;

	start_time = std::chrono::system_clock::now();
  double seq_trap = trapezoidal(0, 1, n);
	duration = std::chrono::system_clock::now() - start_time;
	
	std::cout << "Sequential solution finished in " << duration.count()*1000 << "ms." << std::endl;
	std::cout << "The area is " << seq_trap << " with the sequential solution."<< std::endl;
}
