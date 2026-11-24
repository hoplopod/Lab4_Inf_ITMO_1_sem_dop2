#include <iostream>
#include <exception>

#include "LibConverter.h"

int main()
{
	try {
		LibConverter::convert("schedule.json", "schedule_lib.ini");

	}
	catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
}