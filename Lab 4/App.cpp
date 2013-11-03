#include "App.h"

#include <algorithm>

#include "FrameListener.h"

namespace Lab4 {
App::App() :
	frameListener(nullptr),
	fpsCtrl(nullptr),
	targetFactory(nullptr),
	ballFactory(nullptr)
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

	if (targetFactory != nullptr) {
		delete targetFactory;
	}

	if (ballFactory != nullptr) {
		delete ballFactory;
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
		500, 500,   // size
		250, 250,   // how many quads are used to make the plane
		true,
		1, 5, 5, Vector3::UNIT_Z);

	// Create an instance of the plane
	auto planeEnt = mSceneMgr->createEntity("PlaneEntity", "plane");
	planeEnt->setMaterialName("Examples/BeachStones");

	// Create a node for the plane and attach he plane to it
	mSceneMgr->getRootSceneNode()->createChildSceneNode("planeNode")->attachObject(planeEnt);
#pragma endregion

#pragma region Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue::Black);
#pragma endregion

#pragma region Entities
	auto objNode = mSceneMgr->getSceneNode("planeNode")->createChildSceneNode("objNode");
	targetFactory = new GameObjectFactory(mSceneMgr, objNode);

	// Object A
	targetFactory->makeNew("objA", "barrel.mesh",  Ogre::Vector3(-10.0f,  3.0f,  10.0f));
	// Object B
	targetFactory->makeNew("objB", "barrel.mesh",  Ogre::Vector3( 10.0f,  3.0f,  10.0f));
	// Object C
	targetFactory->makeNew("objC", "Penguin.mesh", Ogre::Vector3(-10.0f,  4.5f, -10.0f));
	targetFactory->get("objC")->getNode()->scale(0.25f, 0.25f, 0.25f);
	// Object D
	targetFactory->makeNew("objD", "Penguin.mesh", Ogre::Vector3( 10.0f,  4.5f, -10.0f));
	targetFactory->get("objD")->getNode()->scale(0.25f, 0.25f, 0.25f);

	/* DEBUG - SHOW BOUND BOX
	auto objs = targetFactory->getAll();
	for (int i = 0; i < objs.size(); ++i)
	{
		auto obj = objs.at(i);
		obj->getNode()->showBoundingBox(true);
	}
	//*/
#pragma endregion

	fpsCtrl = new FPSController(mSceneMgr, mWindow->getViewport(0));
}

void App::createCamera()
{
	mCamera = mSceneMgr->createCamera("cam1");
	mCamera->setNearClipDistance(5);
}

void App::createFrameListener()
{
#pragma region Register
	// Create the custom frame listener
	frameListener = new FrameListener(mWindow, mSceneMgr, fpsCtrl, targetFactory);
	// Register it with the root Ogre class
	mRoot->addFrameListener(frameListener);
#pragma endregion

#pragma region Events
	// Typedefs to make the code neater.
	typedef FrameListener::FrameEventArgs Args;
	typedef std::function<void (Args)> EventFunc;

	std::vector<EventFunc> frameStartedEvents;
	std::vector<EventFunc> renderingQueuedEvents;

	// Update FPS Controller.
	frameStartedEvents.push_back([&] (Args args) {
		if (fpsCtrl != nullptr) fpsCtrl->handleInput(*args->evt, args->key, args->mouse);
	});

	// Calls the update method on all objects of a given factory.
	auto updateObjectFactory = [&](GameObjectFactory* f, Args args) {
		if (f != nullptr) {
			auto objects = f->getAll();
			std::for_each(objects.begin(), objects.end(), [&](GameObject* obj) {
				if (obj != nullptr) obj->update(args->evt);
			});
		}
	};

	// Update Balls.
	renderingQueuedEvents.push_back([&] (Args args) {
		updateObjectFactory(ballFactory, args);
	});

	// Update Targets.
	renderingQueuedEvents.push_back([&](Args args) {
		updateObjectFactory(targetFactory, args);
	});

	// Polygon Mode Toggle
	renderingQueuedEvents.push_back([&] (Args args) {
		if (args->key->isKeyDown(OIS::KC_R)) {
			switch(mCamera->getPolygonMode())
			{
			case Ogre::PolygonMode::PM_SOLID:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
				break;
			case Ogre::PolygonMode::PM_WIREFRAME:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_POINTS);
				break;
			case Ogre::PolygonMode::PM_POINTS:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
				break;
			}
		}
	});
#pragma endregion

#pragma region Register Events
	// The base class frameListener is of type Ogre::FrameLister,
	// so an upcast is needed.
	auto fl = static_cast<Lab4::FrameListener*>(frameListener);

	// Add the functions 
	std::for_each(frameStartedEvents.begin(), frameStartedEvents.end(), [&] (EventFunc f) { 
		fl->OnFrameStarted+= f; 
	});
	std::for_each(renderingQueuedEvents.begin(), renderingQueuedEvents.end(), [&] (EventFunc f) {
		fl->OnFrameRenderingQueued+= f;
	});
#pragma endregion
#pragma endregion
}

GameObject* App::getNextBall()
{
	auto balls = ballFactory->getAll();
	auto result = std::find_if(balls.begin(), balls.end(), [](GameObject* b) { 
		return b->getAlive() == false; 
	});
	return result == balls.end() ? nullptr : *result;
}

} // namespace Lab4