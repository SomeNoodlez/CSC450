/*Multi-threaded counter application
 */

// imports for thread, stdout, and mutex
#include <iostream>
#include <thread>
#include <mutex>
// declare namespace for std
using namespace std;

// wrapper class for thread utilizing RAII
class ThreadRAII{
	// thread reference
	thread& th;
public:
	// constructor initializing th to threadObj
	ThreadRAII(thread& threadObj) : th(threadObj){}
	// destructor, guarantees that thread will be joined if joinable and th goes out of scope
	~ThreadRAII() {
		if(th.joinable()){
			th.join();
		}
	}
};

// class for utilizing multi threaded counter
class ThreadedCounter {
	// declare mutex
	mutex mu;
private:
	// private variables
	int counter, count;
public:
	// constructor to set private variables
	ThreadedCounter(int& n, int& countTo) : counter(n), count(countTo){}

	// getter for the counter and what to count up to
	int getCounter() {return counter;}
	int getCount() {return count;}
	// setter for the value of the counter
	void setCounter(int n){counter = n;}

	// function with mutex protection to guard against race conditions regarding shared variables
	void printer(string msg, int counter){
		lock_guard<mutex> lockGuard(mu);
		// print out message to console with counter
		cout << msg << counter << endl;
	}
};

// function to be threaded, takes in ThreadedCounter object, increments the counter
void incrementCounter(ThreadedCounter& th){
	// while counter is < target value, increment the counter
	while(th.getCounter() <= th.getCount()){ // starts as 0 <= 20
		// print to cout the value of the counter
		th.printer(string("from th1: "), th.getCounter());
		// increment the counter by 1
		th.setCounter(th.getCounter()+1);
	}
}

// function to be threaded, takes in ThreadedCounter object, decrements the counter
void decrementCounter(ThreadedCounter& th){
	// while counter is above 0, decrement the counter until it hits 0
	while(th.getCounter() >= 0){ // initially starts as 21 >= 0
		// if the counter is less than the target value, print to cout value of the counter
		if (th.getCounter() < th.getCount()) th.printer(string("from th2: "), th.getCounter());
		// decrement the counter by 1
		th.setCounter(th.getCounter()-1);
	}
}


int main() {
	// set target value for counter to reach
	int countTo = 20;
	// set value of the counter to be 0
	int i = 0;
	// create new class of ThreadedCounter with parameterized constructor
	ThreadedCounter counter(i, countTo);
	// create two threads with reference to the ThreadedCounter object
	try {
		thread th1(incrementCounter, ref(counter));
		thread th2(decrementCounter, ref(counter));
		// utilize wrapper class to make sure threads are opened and closed properly
		ThreadRAII wrapperThread1(th1);
		ThreadRAII wrapperThread2(th2);
	} catch (...) {
		cerr << "oops something went wrong" << endl;
	}

	return 0;
}
