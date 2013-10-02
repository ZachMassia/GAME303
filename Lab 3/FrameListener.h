#pragma once

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>


namespace Lab3 {
class FrameListener : public Ogre::FrameListener
{
public:
	FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr);
	~FrameListener();

	bool frameStarted(const Ogre::FrameEvent& evt);

private:
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode*    rootSceneNode;
	Ogre::Camera*       camera;

	OIS::InputManager* man;
	OIS::Keyboard*     key;
	OIS::Mouse*        mouse;
};
} // namespace Lab3
