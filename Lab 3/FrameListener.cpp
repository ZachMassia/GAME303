#include "FrameListener.h"

#include <sstream>
#include <utility>


namespace Lab3 {
FrameListener::FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr)
	: sceneMgr(_sceneMgr),
	  rootSceneNode(sceneMgr->getRootSceneNode()),
	  camera(sceneMgr->getCamera("cam1")),
	  man(nullptr),
	  key(nullptr),
	  mouse(nullptr)
{
	size_t windowHandle = 0;
	std::stringstream windowHandleStr;

	// Get the window handle
	win->getCustomAttribute("WINDOW", &windowHandle);
	// Convert the handle to a string
	windowHandleStr << windowHandle;

	// Create the input system using the parameter list
	OIS::ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));
	man = OIS::InputManager::createInputSystem(pl);

	// Create a keyboard
	key = static_cast<OIS::Keyboard*>(man->createInputObject(OIS::OISKeyboard, false));
	// Create a mouse
	mouse = static_cast<OIS::Mouse*>(man->createInputObject(OIS::OISMouse, false));
}

FrameListener::~FrameListener(void)
{
	if (man != nullptr) {
		man->destroyInputObject(key);
		man->destroyInputObject(mouse);
		OIS::InputManager::destroyInputSystem(man);
	}
}

bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
#pragma region Poll devices
	key->capture();   
	mouse->capture(); 

	// Exit on Escape
	if (key->isKeyDown(OIS::KC_ESCAPE)) {
		return false;
	}
#pragma endregion
	
#pragma region Polygon Mode
	// Polygon mode toggle
	if (key->isKeyDown(OIS::KC_R)) {
		// Toggle
		switch(camera->getPolygonMode())
		{
		case Ogre::PolygonMode::PM_SOLID:
			camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
			break;
		case Ogre::PolygonMode::PM_WIREFRAME:
			camera->setPolygonMode(Ogre::PolygonMode::PM_POINTS);
			break;
		case Ogre::PolygonMode::PM_POINTS:
			camera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
			break;
		}
	}
#pragma endregion

#pragma region Camera Movement
	Ogre::Vector3 translate(0, 0, 0);
	Ogre::Real vel(35);
	if (key->isKeyDown(OIS::KC_W)) { // forward
		translate += Ogre::Vector3(0, 0, -1);
	}
	if (key->isKeyDown(OIS::KC_S)) { // backwards
		translate += Ogre::Vector3(0, 0, 1);
	}
	if (key->isKeyDown(OIS::KC_A)) { // left
		translate += Ogre::Vector3(-1, 0, 0);
	}
	if (key->isKeyDown(OIS::KC_D)) { // right
		translate += Ogre::Vector3(1, 0, 0);
	}
	if (key->isKeyDown(OIS::KC_E)) { // up
		translate += Ogre::Vector3(0, 1, 0);
	}
	if (key->isKeyDown(OIS::KC_Q)) { // down
		translate += Ogre::Vector3(0, -1, 0);
	}

	// Camera mouse
	float rotX = mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -0.5;
	float rotY = mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -0.5;
	
	camera->yaw(Ogre::Radian(rotX));
	camera->pitch(Ogre::Radian(rotY));
	camera->moveRelative(translate * evt.timeSinceLastFrame * vel);
#pragma endregion

#pragma region Spotlight Movement
	Ogre::Vector3 spotDir = sceneMgr->getLight("spotLight")->getDirection();

	if (key->isKeyDown(OIS::KC_UP)) {
		translate += Ogre::Vector3(0, 0, -1);
	}
	if (key->isKeyDown(OIS::KC_DOWN)) {
		translate += Ogre::Vector3(0, 0, 1);
	}
	if (key->isKeyDown(OIS::KC_LEFT)) {
		translate += Ogre::Vector3(-1, 0, 0);
	}
	if (key->isKeyDown(OIS::KC_RIGHT)) {
		translate += Ogre::Vector3(1, 0, 0);
	}
	// Apply the new direction to the spotlight. 
	sceneMgr->getLight("spotLight")->setDirection(spotDir);
#pragma endregion

#pragma region Rotate Entities
	static const Ogre::Real velocity(75.0f);
	rootSceneNode->getChild("sinbadNode")->yaw(Ogre::Degree(velocity * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
#pragma endregion

	return true;
}
} // namespace Lab3