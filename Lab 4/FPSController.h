#pragma once

#include <OGRE\Ogre.h>
#include <OIS/OIS.h>

namespace Lab4 {
// This class is based on http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Creating+a+simple+first-person+camera+system
class FPSController
{
public:
	FPSController(Ogre::SceneManager* _sceneMgr);
	~FPSController(void);

	void handleInput(const Ogre::FrameEvent& evt, OIS::Keyboard* key, OIS::Mouse* mouse);
	

private:
	void moveCamera(Ogre::Real dt);
	void updateRay();
	void createLights();
	void setUpNodeHierarchy();

	Ogre::SceneManager* sceneMgr;
	
	Ogre::SceneNode* cameraYawNode;
	Ogre::SceneNode* cameraPitchNode;
	Ogre::SceneNode* cameraRollNode;
	Ogre::SceneNode* cameraNode;

	Ogre::Vector3 accel, velocity;

	Ogre::Radian camYaw, camPitch;

	Ogre::Ray camRay;
	Ogre::ManualObject* camRayObj;

	Ogre::Radian rotX, rotY;
};
} // namespace Lab4
