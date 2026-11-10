#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

/* using a few concepts I added a clock and a method to measuring runtimes
 * given the formula:
 * Tturnaround = Tcompletion - Tarrival
 *
 * This Formula can be found on page 74 of Operating Systems Three Easy Pieces. */

using namespace std;

mutex Mutex_output;


void task(int id, int seconds) {
	auto arrival = chrono::steady_clock::now();

	{
		lock_guard<mutex> lock(Mutex_output);
		cout << "Thread " << id << " started\n";
	}

	// using the clock and turnaround formula.

	this_thread::sleep_for(chrono::seconds(seconds));

	auto completion = chrono::steady_clock::now();

	chrono::duration<double> turnaround = completion - arrival;

	{
		lock_guard<mutex> lock(Mutex_output);

		cout << "Thread " << id << " finished!\n";

		cout << "Thread " << id
			<< " turnaround: "
			<< turnaround.count()
			<< " seconds\n\n\n\n";

	}
}


int main() {

	auto start = chrono::steady_clock::now();
	// SEQUENTIAL RUNTIMES
	thread t1(task, 1, 3);
	t1.join();
	thread t2(task, 2, 7);
	t2.join();
	thread t3(task, 3, 9);
	t3.join();
	thread t4(task, 4, 10);
	t4.join();
	thread t5(task, 5, 3);
	t5.join();

	
	/* Before I added the joins below the thread or task, threads 1-5 executed
	 * all at once, where the operating system keeps running the other tasks
	 * until the current task is completed. This method offered a run time of 10 seconds.
	 *
	 * where now I joined the thread as soon as each is executed so each thread processes
	 * one at a time which creates the runtime of 30 seconds. */


	// THE CONCURRENT RUNTIMES
	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
	//t5.join();

	auto finish = chrono::steady_clock::now();

	chrono::duration<double> elapsed = finish - start;


	cout << "total runtime: " << elapsed.count() << " seconds" << endl;

		return 0;

}
