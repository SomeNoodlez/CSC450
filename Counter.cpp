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

//class ThreadedCounter {
//	mutex mu;
//	condition_variable cond;
//	bool loaded;
//private: int counter, count;
//public:
//	ThreadedCounter(int &n, int &countTo) : counter(n), count(countTo) {
//		loaded = false;
//	}
//	// function to be threaded, takes in ThreadedCounter object, increments the counter
//	void incrementCounter(){
//		lock_guard<mutex> lockGuard(mu);
//		int n = 0;
//		while (n <= 1000){
//			cout << "from th1: " << n << endl;
//			n++;
//		}
//		loaded = true;
//		cond.notify_one();
//	}
//	// function to be threaded, takes in ThreadedCounter object, decrements the counter
//	void decrementCounter(){
//		int n = 1000;
//		this_thread::sleep_for(chrono::milliseconds(1000));
//		unique_lock<mutex> mlock(mu);
//		cond.wait(mlock, bind(&ThreadedCounter::incrementCounter, this));
//		while (n >= 0){
//			if (n < 1000) cout << "from th2: " << n << endl;
//			n--;
//		}
//	}
//};

//class ThreadedCounter {
//	mutex mu;
//public:
//	ThreadedCounter(){}
//	void sharedPrint(string msg, int n){
//		lock_guard<mutex> lockGuard(mu);
//		cout << msg << n << endl;
//	}
//};
mutex mu;
int i = 0;
condition_variable cond;


void incrementCounter() {
	int count = 10000;
	lock_guard<mutex> lockGuard(mu);
	while (i < count){
		cout << "from th1: " << i << endl;
		i++;
	}
	cond.notify_one();
}

void decrementCounter() {
	int count = 0;
	unique_lock<mutex> locker(mu);
	cond.wait(locker, []{return (i != 0) ? true: false;});
	while (i >= count) {
		cout << "from th2: " << i << endl;
		i--;
	}
//	int i = 1000;
//	int count = 0;
//
//	while (i >= count) {
//		unique_lock<mutex> locker(mu);
//		cond.wait(locker, [](){return i != 0;});
//		cout << "from th2: " << i <<endl;
//		i--;
//		locker.unlock();
//
//	}
}



int main() {
//	ThreadedCounter counter;
	// set target value for counter to reach
//	int countTo = 1000;
	// set value of the counter to be 0
//	int i = 0;
//	ThreadedCounter counter(i, countTo);
	try {
//		thread th1(incrementCounter, ref(i), ref(countTo));
//		thread th2(decrementCounter, ref(i), ref(countTo));
//		thread th1(&ThreadedCounter::incrementCounter, ref(counter));
//		thread th2(&ThreadedCounter::decrementCounter, ref(counter));
		thread th1(incrementCounter);
		thread th2(decrementCounter);
//		th1.join();
//		th2.join();
		ThreadRAII wrapperThread1(th1);
		ThreadRAII wrapperThread2(th2);
	} catch (...) {
		cerr << "oops something went wrong" << endl;
	}

	return 0;
}
