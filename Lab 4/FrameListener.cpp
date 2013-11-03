#include "FrameListener.h"

#include "Utils.h"

#include <sstream>
#include <algorithm>
#include <utility>


namespace Lab4 {
FrameListener::FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr, FPSController* _fpsCtrl, GameObjectFactory* _targetFactory)
	: sceneMgr(_sceneMgr),
	  camera(sceneMgr->getCamera("cam1")),
	  man(nullptr),
	  key(nullptr),
	  mouse(nullptr),
	  fpsCtrl(_fpsCtrl),
	  targetFactory(_targetFactory)
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

	OnFrameStarted.RaiseEvent(new _FrameEventArgs(key, mouse, &evt));

#pragma region Collisions
	auto objects = targetFactory->getAll();
	// Check if the current GameObject is in the camera ray's path.
	// The object's axis aligned bounding box is passed to the ray.
	std::for_each(objects.begin(), objects.end(), [&] (GameObject* obj) 
	{
		auto result = fpsCtrl->getCameraRay().intersects(obj->getAABB());
		if (result.first) {
			obj->setAlive(false);
		}
	});
#pragma endregion
	return true;
}

bool FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	key->capture();   
	mouse->capture(); 

	// Exit on Escape
	if (key->isKeyDown(OIS::KC_ESCAPE)) {
		return false;
	}

	OnFrameRenderingQueued.RaiseEvent(new _FrameEventArgs(key, mouse, &evt));

	return true;
}
} // namespace Lab4