#include "App.h"

#include <iostream>
#include <exception>

int main()
{
	Lab3::App app;
	try {
		app.go();
	} catch (std::exception e) {
		std::cerr << "Exception occured: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}