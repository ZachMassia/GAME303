#include "FPSController.h"

#include "Utils.h"

#include <iostream>
#include <sstream>


namespace Lab4 {
FPSController::FPSController(Ogre::SceneManager* _sceneMgr, Ogre::Viewport* _viewPort) :
	sceneMgr(_sceneMgr),
	viewPort(_viewPort),
	cam1(sceneMgr->getCamera("cam1")),
	cameraYawNode(nullptr),
	cameraPitchNode(nullptr),
	cameraRollNode(nullptr),
	cameraNode(nullptr),
	accel(Ogre::Vector3::ZERO),
	velocity(50, 50, 50)
{
	setUpNodeHierarchy();
	createLights();
}

FPSController::~FPSController(void)
{
}

void FPSController::handleInput(const Ogre::FrameEvent& evt, OIS::Keyboard* key, OIS::Mouse* mouse)
{
	// Get the mouse movement.
	rotX = Ogre::Radian(mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -0.5);
	rotY = Ogre::Radian(mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -0.5);

	accel = Ogre::Vector3::ZERO;

	if (key->isKeyDown(OIS::KC_W)) { // forward
		accel.z = -1;
	}
	if (key->isKeyDown(OIS::KC_S)) { // backwards
		accel.z = 1;
	}
	if (key->isKeyDown(OIS::KC_A)) { // left
		accel.x = -1;
	}
	if (key->isKeyDown(OIS::KC_D)) { // right
		accel.x = 1;
	}
#pragma region Debug Controls
	if (key->isKeyDown(OIS::KC_Q)) {
		accel.y = -1;
	}
	if (key->isKeyDown(OIS::KC_E)) {
		accel.y = 1;
	}
	if (key->isKeyDown(OIS::KC_B)) {
		std::cout << cameraNode->getPosition() << std::endl;
	}
#pragma endregion
	moveCamera(evt.timeSinceLastFrame);
}

const Ogre::Ray& FPSController::getCameraRay() const
{
	return camRay;
}

const Ogre::AxisAlignedBox& FPSController::getCameraAABB() const
{
	return cam1->getWorldBoundingBox();
}

const Ogre::Vector3& FPSController::getWorldPosition() const
{
	return cameraNode->_getDerivedPosition();
}

void FPSController::moveCamera(Ogre::Real dt)
{
	Ogre::Real pitchAngle, pitchAngleSign;

	// Yaw the camera according to the mouse relative movement.
	cameraYawNode->yaw(rotX);

	// Pitch the camera according to the mouse relative movement.
	cameraPitchNode->pitch(rotY);

	// Move the camera around the world.
	cameraNode->translate(cameraYawNode->getOrientation() *
						  cameraPitchNode->getOrientation() *
						  accel * velocity * dt, // TODO: Normalize for diagonal movement.
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

	// Keep the camera within the playing area.
	auto pos = cameraNode->getPosition();
	Utils::clampVector(pos, Ogre::Vector3(-250, 7.5f, -250), Ogre::Vector3(250, 35, 250));
	cameraNode->setPosition(pos);

	// Notify cameraNode's children.
	cameraYawNode->needUpdate();
	cameraPitchNode->needUpdate();
	cameraRollNode->needUpdate();

	// Update the camera ray.
	camRay.setOrigin(cam1->getRealPosition());
	camRay.setDirection(cam1->getRealDirection());
}

void FPSController::createLights()
{
	// Spotlight
	auto light = sceneMgr->createLight("playerSpotlight");
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setSpotlightRange(
		Ogre::Degree(20.0f), // inner angle
		Ogre::Degree(45.0f), // outer angle
		0.25f);              // falloff

	// Laser
	/*auto laser = sceneMgr->createLight("playerLaser");
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setSpotlightRange(
		Ogre::Degree(2.5f), // inner angle
		Ogre::Degree(5.0f), // outer angle
		1.25f);             // falloff
	*/
	auto node = cameraRollNode->createChildSceneNode("lightNode");
	node->attachObject(light);
	//node->attachObject(laser);
	node->yaw(Ogre::Degree(180));
}

void FPSController::setUpNodeHierarchy()
{
	// Create the camera's top node (which will only handle position).
	cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode("cameraRoot");
	cameraNode->setPosition(0, 7.5, 125);
 
	// Create the camera's yaw node as a child of camera's top node.
	cameraYawNode = cameraNode->createChildSceneNode("cameraYaw");
 
	// Create the camera's pitch node as a child of camera's yaw node.
	cameraPitchNode = cameraYawNode->createChildSceneNode("cameraPitch");
 
	// Create the camera's roll node as a child of camera's pitch node
	// and attach the camera to it.
	cameraRollNode = cameraPitchNode->createChildSceneNode("cameraRoll");
	cameraRollNode->attachObject(cam1);
}
} // namespace Lab4
