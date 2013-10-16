#include "FPSController.h"

#include <iostream>

namespace Lab4 {
FPSController::FPSController(Ogre::SceneManager* _sceneMgr) :
	sceneMgr(_sceneMgr),
	cameraYawNode(nullptr),
	cameraPitchNode(nullptr),
	cameraRollNode(nullptr),
	cameraNode(nullptr),
	direction(Ogre::Vector3::ZERO),
	velocity(Ogre::Vector3(25, 15, 25))
{
	setUpNodeHierarchy();
}


FPSController::~FPSController(void)
{
}

void FPSController::handleInput(const Ogre::FrameEvent& evt, OIS::Keyboard* key, OIS::Mouse* mouse)
{
	// Get the mouse movement.
	rotX = mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -0.5;
	rotY = mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -0.5;

	direction = Ogre::Vector3::ZERO; // Reset the vector

	if (key->isKeyDown(OIS::KC_W)) { // forward
		direction.z = -1;
	}
	if (key->isKeyDown(OIS::KC_S)) { // backwards
		direction.z = 1;
	}
	if (key->isKeyDown(OIS::KC_A)) { // left
		direction.x = -1;
	}
	if (key->isKeyDown(OIS::KC_D)) { // right
		direction.x = 1;
	}
#pragma region Debug Controls
	if (key->isKeyDown(OIS::KC_Q)) {
		direction.y = -1;
	}
	if (key->isKeyDown(OIS::KC_E)) {
		direction.y = 1;
	}
	if (key->isKeyDown(OIS::KC_B)) {
		std::cout << cameraNode->getPosition() << std::endl;
	}
#pragma endregion
	moveCamera(evt.timeSinceLastFrame);
}

void FPSController::moveCamera(Ogre::Real dt)
{
	Ogre::Real pitchAngle, pitchAngleSign;

	// Yaw the camera according to the mouse relative movement.
	cameraYawNode->yaw(Ogre::Radian(rotX));

	// Pitch the camera according to the mouse relative movement.
	cameraPitchNode->pitch(Ogre::Radian(rotY));

	// Translate the camera according to the translate vector which is
	// controlled by the keyboard arrows.
	cameraNode->translate(cameraYawNode->getOrientation() *
						  cameraPitchNode->getOrientation() *
						  direction * velocity * dt,
						  Ogre::Node::TS_LOCAL);

	// Angle of rotation around the X-Axis.
	pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(cameraPitchNode->getOrientation().w)).valueDegrees());

	// Just to determine the sign of the angle we pick up above, the value
	// itself does not interest us.
	pitchAngleSign = cameraPitchNode->getOrientation().x;

	// Limit the pitch between -90 and +90 degrees, Quake3 style.
	if (pitchAngle > 90.0f) {
		if (pitchAngleSign > 0) {
			// Set orientation to 90 degrees on X-Axis.
			cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
		} else if (pitchAngleSign < 0) {
			// Set orientation to -90 degrees on X-Axis.
			cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
		}
	} 
}

void FPSController::setUpNodeHierarchy()
{
	// Create the camera's top node (which will only handle position).
	cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	cameraNode->setPosition(0, 0, 500);
 
	// Create the camera's yaw node as a child of camera's top node.
	cameraYawNode = cameraNode->createChildSceneNode();
 
	// Create the camera's pitch node as a child of camera's yaw node.
	cameraPitchNode = cameraYawNode->createChildSceneNode();
 
	// Create the camera's roll node as a child of camera's pitch node
	// and attach the camera to it.
	cameraRollNode = cameraPitchNode->createChildSceneNode();
	cameraRollNode->attachObject(sceneMgr->getCamera("cam1"));
}
} // namespace Lab4
