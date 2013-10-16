#include "FrameListener.h"

#include <sstream>
#include <utility>


namespace Lab4 {
FrameListener::FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr, FPSController* _fpsCtrl)
	: sceneMgr(_sceneMgr),
	  camera(sceneMgr->getCamera("cam1")),
	  man(nullptr),
	  key(nullptr),
	  mouse(nullptr),
	  fpsCtrl(_fpsCtrl)
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

	if (fpsCtrl == nullptr) {
		return false;
	}
	fpsCtrl->handleInput(evt, key, mouse);

/*
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
	
	auto playerNode = sceneMgr->getSceneNode("playerNode");

	playerNode->yaw(Ogre::Radian(rotX));
	playerNode->pitch(Ogre::Radian(rotY));
	playerNode->translate(translate * evt.timeSinceLastFrame * vel, Ogre::Node::TS_WORLD);
#pragma endregion
*/
	return true;
}
} // namespace Lab4