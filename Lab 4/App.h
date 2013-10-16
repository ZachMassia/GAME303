#pragma once

#include "FPSController.h"

#include <OGRE/Ogre.h>
#include <OGRE/ExampleApplication.h>

namespace Lab4 {
class App : public ExampleApplication
{
public:
	App();
	~App();

	void createScene();
	void createCamera();
	void createFrameListener();

private:
	Ogre::FrameListener* frameListener;
	FPSController* fpsCtrl;
};
} // namespace Lab4

