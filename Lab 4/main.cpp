#include "App.h"

#include <iostream>
#include <exception>
#include <OGRE/OgreException.h>

int main()
{
	Lab3::App app;

	// Launches the application, and catches any expection passed.
	try {
		app.go();
	} catch (Ogre::Exception e) {
		std::cerr << "OGRE Exception occured: " << e.getFullDescription() << std::endl;
		return 1;
	} catch (std::exception e) {
		std::cerr << "Exception occured: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}