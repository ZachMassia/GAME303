#include "App.h"

#include <array>

#include "FrameListener.h"

namespace Lab3 {
App::App() :
	frameListener(nullptr)
{

}

App::~App()
{
	if (frameListener != nullptr) {
		delete frameListener;
	}
}

void App::createScene()
{
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
	// Directional
	auto sunlight = mSceneMgr->createLight("sun");
	sunlight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunlight->setDirection(Ogre::Vector3(0, -1, -1));
	sunlight->setDiffuseColour(Ogre::ColourValue(.30, .30, 0));
	sunlight->setSpecularColour(Ogre::ColourValue(.30, .30, 0));

	// Spotlight
	auto spotlight = mSceneMgr->createLight("spotlight");
	spotlight->setType(Ogre::Light::LT_SPOTLIGHT);
	
	spotlight->setSpotlightRange(
		Ogre::Degree(5.0f),  // inner angle
		Ogre::Degree(15.0f), // outer angle
		0.0f);               // falloff
	
	spotlight->setDiffuseColour(Ogre::ColourValue(1.0f, 0.0f, 0.0f));

	// Sphere to visualize the spotlights source
	auto sphereEnt = mSceneMgr->createEntity("sphere", "sphere.mesh");
	sphereEnt->setMaterialName("Examples/checker");
	auto sphereNode = mSceneMgr->getSceneNode("planeNode")->createChildSceneNode("spotlightNode");
	sphereNode->attachObject(sphereEnt);
	sphereNode->attachObject(spotlight);
	sphereNode->scale(0.02f, 0.02f, 0.02f);
	sphereNode->translate(0.0f, 15.0f, 0.0f);
	sphereNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
#pragma endregion

#pragma region Entities
	std::array<Ogre::Entity*, 6> entities;
	auto entParentNode = mSceneMgr->getSceneNode("planeNode")->createChildSceneNode("entParentNode");

	float angleOffset = 360.0f / 6.0f;
	float radius = 30.0f;

	for (int i = 0; i < entities.size(); ++i)
	{
		auto e = mSceneMgr->createEntity("Sinbad.mesh");
		entParentNode->createChildSceneNode(Ogre::Vector3(
			radius * Math::Cos(Math::DegreesToRadians(angleOffset * i)),  // x = r cos(t)
			6.75f,                                                        // y = height
			radius * Math::Sin(Math::DegreesToRadians(angleOffset * i)))) // z = r sin(t)
		->attachObject(e);
	}

	// Barrel
	auto barrel = mSceneMgr->createEntity("barrel.mesh");
	mSceneMgr->getSceneNode("planeNode")->createChildSceneNode("barrel", Ogre::Vector3(0, 2.5f, 0))->attachObject(barrel);
#pragma endregion

	// Skybox
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 5000, false);
}

void App::createCamera()
{
	mCamera = mSceneMgr->createCamera("cam1");
	mCamera->setPosition(0, 100, 150);
	mCamera->lookAt(0.0f, 0.0f, 0.0f);
	mCamera->setNearClipDistance(5);
}

void App::createFrameListener()
{
	// Create the custom frame listener
	frameListener = new FrameListener(mWindow, mSceneMgr);
	// Register it with the root Ogre class
	mRoot->addFrameListener(frameListener);
}
} // namespace Lab3