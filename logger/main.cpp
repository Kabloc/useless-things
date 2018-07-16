#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

#include "logger.h"

logger main_log;

void function(const int& x ) {
	int w = 1 + std::rand() / ((RAND_MAX + 1u) / 10);
	std::this_thread::sleep_for(std::chrono::milliseconds(w));
	for (int c = 0; c <= 1000; c++) {
		LOGGER_OUT(main_log, logNONE) << "### IT WONT BE LOGGED ### [" << x << "]:" << c;
		LOGGER_OUT(main_log, logERROR)   << "Log error number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logWARNING) << "Log warning number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logINFO)    << "Log information number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logDEBUG1)  << "Log debug 1 number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logDEBUG2)  << "Log debug 2 number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logDEBUG3)  << "Log debug 3 number[" << x << "]:" << c;
		LOGGER_OUT(main_log, logDEBUG4)  << "Log debug 4 number[" << x << "]:" << c;
	}
}

int main(int argc, char* argv[]) {
	main_log.start_logging("logger_test.log", 1024*500, 8, logERROR, false, false, "my_module" );

	std::vector<std::thread> threads;
	for (int x = 0; x <= 10; x++) 
		threads.push_back(std::thread([x]() { function(x); }));

	for (auto &a : threads)
		a.join();

	return 1;
}
