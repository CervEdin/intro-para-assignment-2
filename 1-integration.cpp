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

// Parallel solution to the problem
void trap_area(double start, double end, double width, int n)
{
	double partial_area = 0.0;
	double a = start;
	double b = start + width;

	while(a < end){
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

// Sequential solution to the problem
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
	return sum;
}

void usage(char *program)
{
	std::cout << "Usage: " << program << " T Trapz" << std::endl;
	std::cout << std::endl;
	std::cout << "  -h: Display help" << std::endl;
	std::cout << "  T: Nr of Threads" << std::endl;
	std::cout << "  Trapz: Nr of Trapezes" << std::endl;
	exit(1);
}

int main(int argc, char *argv[])
{
	std::string help = argv[1];
	if (!help.compare("-h") || argc != 3)
	{
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
	// Trapezoid width
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

	//std::cout << "N = " << n << " T = " << T << std::endl;
  //std::cout << "Parallel solution finished in "  << duration.count()*1000 << "ms. Area = " << area << std::endl;
	std::cout << " & " << duration.count()*1000 << "ms. area=" << area << " " << std::endl;

	//start_time = std::chrono::system_clock::now();
  //double seq_trap = trapezoidal(0, 1, n);
	//duration = std::chrono::system_clock::now() - start_time;

	//std::cout << "Sequential solution finished in " << duration.count()*1000 << "ms. Area = " << seq_trap << std::endl;
}
