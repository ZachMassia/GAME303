#include "App.h"

#include <array>

#include "FrameListener.h"

namespace Lab4 {
App::App() :
	frameListener(nullptr),
	fpsCtrl(nullptr)
{

}

App::~App()
{
	if (frameListener != nullptr) {
		delete frameListener;
	}
	if (fpsCtrl != nullptr) {
		delete fpsCtrl;
	}
}

void App::createScene()
{
	fpsCtrl = new FPSController(mSceneMgr);

	// Skybox
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 5000, false);

	// Create a scene node for the camera and the players light. It will
	// have no childs and the root node is it's parent.
	mSceneMgr->getRootSceneNode()->createChildSceneNode("playerNode");

#pragma region Plane
	// Define the mathematical plane
	Ogre::Plane plane(Vector3::UNIT_Y, 0);
	
	// Create the plane into memory
	Ogre::MeshManager::getSingleton().createPlane(
		"plane",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		500, 500, // size
		25, 25,   // how many quads are used to make the plane
		true,
		1, 5, 5, Vector3::UNIT_Z);

	// Create an instance of the plane
	auto planeEnt = mSceneMgr->createEntity("PlaneEntity", "plane");
	planeEnt->setMaterialName("Examples/BeachStones");

	// Create a node for the plane and attach he plane to it
	mSceneMgr->getRootSceneNode()->createChildSceneNode("planeNode")->attachObject(planeEnt);
#pragma endregion

#pragma region Lights
	auto playerSpot = mSceneMgr->createLight("playerSpotlight");
	playerSpot->setType(Ogre::Light::LT_SPOTLIGHT);
	playerSpot->setSpotlightRange(
		Ogre::Degree(10.0f), // inner angle
		Ogre::Degree(35.0f), // outer angle
		0.75f);              // falloff
	playerSpot->setDiffuseColour(Ogre::ColourValue(1, 0, 0));
#pragma endregion
}

void App::createCamera()
{
	mCamera = mSceneMgr->createCamera("cam1");
	mCamera->setNearClipDistance(5);
}

void App::createFrameListener()
{
	// Create the custom frame listener
	frameListener = new FrameListener(mWindow, mSceneMgr, fpsCtrl);
	// Register it with the root Ogre class
	mRoot->addFrameListener(frameListener);
}
} // namespace Lab4