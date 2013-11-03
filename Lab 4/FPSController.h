#pragma once

#include <OGRE\Ogre.h>
#include <OIS/OIS.h>
#include <utility>
#include <string>

namespace Lab4 {
// This class is based on http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Creating+a+simple+first-person+camera+system
class FPSController
{
public:
	FPSController(Ogre::SceneManager* _sceneMgr, Ogre::Viewport* _viewPort);
	~FPSController(void);

	void handleInput(const Ogre::FrameEvent& evt, OIS::Keyboard* key, OIS::Mouse* mouse);
	const Ogre::Ray& getCameraRay() const;

private:
	void moveCamera(Ogre::Real dt);
	void updateRay();
	void createLights();
	void setUpNodeHierarchy();

	Ogre::SceneManager* sceneMgr;
	Ogre::Viewport* viewPort;
	Ogre::Camera* cam1;
	
	Ogre::SceneNode* cameraYawNode;
	Ogre::SceneNode* cameraPitchNode;
	Ogre::SceneNode* cameraRollNode;
	Ogre::SceneNode* cameraNode;

	Ogre::Vector3 accel, velocity;

	Ogre::Ray camRay;

	Ogre::Radian rotX, rotY;
};
} // namespace Lab4
