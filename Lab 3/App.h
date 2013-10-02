#pragma once

#include <OGRE/Ogre.h>
#include <OGRE/ExampleApplication.h>

namespace Lab3 {
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
};
} // namespace Lab3

