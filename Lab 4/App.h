#pragma once

#include "FPSController.h"
#include "GameObject.h"

#include <array>
#include <OGRE/Ogre.h>
#include <OGRE/ExampleApplication.h>


namespace Lab4 {
class App : public ExampleApplication
{
private:
	Ogre::FrameListener* frameListener;
	FPSController* fpsCtrl;

	GameObjectFactory* targetFactory;
	GameObjectFactory* ballFactory;

public:
	App();
	~App();

	void createScene();
	void createCamera();
	void createFrameListener();

private:
	GameObject* getNextBall();
};
} // namespace Lab4

