// Programmers at the Flaky Computer Corporation designed the protocol shown below to achieve n-thread mutual exclusion.
// For each question, either sketch a proof, or display an execution where it fails.
// 1. Does this protocol satisfy mutual exclusion?
// 2. Is this protocol starvation-free?
// 3. Is this protocol deadlock-free?

#include <iostream>
#include <thread>
#include <mutex>

//std::mutex my_turn;
int shared = 0;

class Flaky {
  private:
    std::thread::id turn ;
	  bool busy = false;
	public:
    void lock() {
  		std::thread::id me = std::this_thread::get_id();
  		do {
  			do {
  				turn = me;
  			} while ( busy ) ;
  			busy = true;
  		} while ( turn != me ) ;
	  }
	  void unlock () {
		  busy = false;
	  }
};

void test(Flaky flake)
{
  std::string temp = "flaky", test_string = "     ";
  flake.lock();
  for(int i=0;i<temp.length();i++){
    test_string[i] = temp[i];
  }
  std::cout << test_string << std::endl;
  flake.unlock();
}

int main(int argc, char *argv[])
{
  int threads = std::stoi(argv[1]);

  Flaky flake;

  std::thread *t = new std::thread[threads];

  for(int i=0; i<threads; i++){
    t[i] = std::thread(test, flake);
  }

  for(int i=0; i<threads; i++){
    t[i].join();
  }
}
