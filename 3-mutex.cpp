// Programmers at the Flaky Computer Corporation designed the protocol shown below to achieve n-thread mutual exclusion.
/*
class Flaky implements Lock {
	private int turn ;
	private boolean busy = false;
	public void lock() {
		int me = ThreadID.get() ;
		do {
			do {
				turn = me;
			} while ( busy ) ;
			busy = true;
		} while ( turn != me ) ;
	}
	public void unlock () {
		busy = false;
	}
}
*/

// For each question, either sketch a proof, or display an execution where it fails.
// 1. Does this protocol satisfy mutual exclusion?
// 2. Is this protocol starvation-free?
// 3. Is this protocol deadlock-free?

#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
}
