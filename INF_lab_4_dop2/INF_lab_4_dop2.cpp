#include <iostream>
#include <chrono>
#include <exception>

#include "LibConverter.h"

const int RUNS = 100;

int main()
{
	try {

		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < RUNS; i++) LibConverter::convert("schedule.json", "schedule_lib.ini");
		auto end = std::chrono::steady_clock::now();

		double libTime = std::chrono::duration<double, std::milli>(end - start).count();
		std::cout << "Libs JSON -> INI (" << RUNS << "x): " << libTime << " ms\n";
	}
	catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
}