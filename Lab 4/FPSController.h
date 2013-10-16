#pragma once

#include <OGRE\Ogre.h>
#include <OIS/OIS.h>

namespace Lab4 {
class FPSController
{
public:
	FPSController(Ogre::SceneManager* _sceneMgr);
	~FPSController(void);

	void handleInput(const Ogre::FrameEvent& evt, OIS::Keyboard* key, OIS::Mouse* mouse);

private:
	void moveCamera(Ogre::Real dt);
	void setUpNodeHierarchy();

	Ogre::SceneManager* sceneMgr;
	
	Ogre::SceneNode *cameraYawNode;
	Ogre::SceneNode *cameraPitchNode;
	Ogre::SceneNode *cameraRollNode;
	Ogre::SceneNode *cameraNode;

	Ogre::Vector3 direction, velocity;
	float rotX, rotY;
};
} // namespace Lab4
