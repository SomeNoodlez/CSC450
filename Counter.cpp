/*Multi-threaded counter application
 */

// imports for thread, stdout, and mutex
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
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

class ThreadedCounter {
	mutex mu;
	condition_variable cond;
private: int counter, count;
public:
	ThreadedCounter(int n, int countTo) : counter(n), count(countTo) {}
	void setCounter(int n){counter = n;}
	int getCounter(){return counter;}
	int getCount(){return count;}

	void incrementCounter() {
		lock_guard<mutex> lockGuard(mu);
		while (getCounter() < getCount()){
			cout << "from th1: " << getCounter() << endl;
			setCounter(getCounter() + 1);
		}
		cond.notify_one();
	}

	void decrementCounter() {
		unique_lock<mutex> locker(mu);
		cond.wait(locker, [&]{return (getCounter() != 0) ? true: false;});
		while (getCounter() >= 0) {
			cout << "from th2: " << getCounter() << endl;
			setCounter(getCounter() - 1);
		}
	}

};

int main() {	ThreadedCounter counter(0, 1000);

	try {
		thread th1(&ThreadedCounter::incrementCounter, ref(counter));		thread th2(&ThreadedCounter::decrementCounter, ref(counter));

		ThreadRAII wrapperThread1(th1);
		ThreadRAII wrapperThread2(th2);
	} catch (...) {
		cerr << "oops something went wrong" << endl;
	}

	return 0;
}
