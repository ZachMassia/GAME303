#pragma once

#include "FPSController.h"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>


namespace Lab4 {
class FrameListener : public Ogre::FrameListener
{
public:
	FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr, FPSController* _fpsCtrl);
	~FrameListener();

	bool frameStarted(const Ogre::FrameEvent& evt);

private:
	Ogre::SceneManager* sceneMgr;
	Ogre::Camera*       camera;

	OIS::InputManager* man;
	OIS::Keyboard*     key;
	OIS::Mouse*        mouse;

	FPSController* fpsCtrl;
};
} // namespace Lab4
