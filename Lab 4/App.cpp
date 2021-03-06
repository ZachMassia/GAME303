#include "App.h"

#include <algorithm>

#include "FrameListener.h"

namespace Lab4 {
App::App() :
	frameListener(nullptr),
	fpsCtrl(nullptr),
	targetFactory(nullptr),
	ballFactory(nullptr),
	timer(nullptr)
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
	//mSceneMgr->setAmbientLight(Ogre::ColourValue::Black);
	mSceneMgr->setAmbientLight(Ogre::ColourValue::White);
#pragma endregion

#pragma region Entities
	auto objNode = mSceneMgr->getSceneNode("planeNode")->createChildSceneNode("objNode");
	
	// -- Create Targets -- //
	targetFactory = new GameObjectFactory(mSceneMgr, objNode);

	// Barrels
	auto barrel1 = targetFactory->makeNew("barrel1", "barrel.mesh",  Ogre::Vector3(-10.0f, 3.0f, 10.0f));
	
	auto barrel2 = targetFactory->makeNew("barrel2", "barrel.mesh",  Ogre::Vector3(10.0f, 3.0f, 10.0f));
	
	// Penguins
	const Ogre::Vector3 penguinScale(0.25f, 0.25f, 0.25f);
	
	auto penguin1 = targetFactory->makeNew("penguin1", "Penguin.mesh", Ogre::Vector3(-10.0f, 4.5f, -10.0f));
	penguin1->getNode()->setScale(penguinScale);
	
	auto penguin2 = targetFactory->makeNew("penguin2", "Penguin.mesh", Ogre::Vector3(10.0f, 4.5f, -10.0f));
	penguin2->getNode()->setScale(penguinScale);

	// Cubes
	const Ogre::Vector3 cubeScale(0.35f, 0.35f, 0.35f);
	
	auto cube1 = targetFactory->makeNew("cube1", "cube.mesh", Ogre::Vector3(20.0f, 3.0f, -100.0f));
	cube1->getNode()->setScale(cubeScale);

	auto cube2 = targetFactory->makeNew("cube2", "cube.mesh", Ogre::Vector3(-45.0f, 3.0f, -60.0f));
	cube2->getNode()->setScale(cubeScale);

	// Ninjas
	const Ogre::Vector3 ninjaScale(0.10f, 0.10f, 0.10f);

	auto ninja1 = targetFactory->makeNew("ninja1", "ninja.mesh", Ogre::Vector3(100.0f, 3.0f, -25.0f));
	ninja1->getNode()->setScale(ninjaScale);

	auto ninja2 = targetFactory->makeNew("ninja2", "ninja.mesh", Ogre::Vector3(-75.0f, 3.0f, 100.0f));
	ninja2->getNode()->setScale(ninjaScale);

	// -- Create Balls -- //
	ballFactory = new GameObjectFactory(mSceneMgr, objNode);
	char id = 'A';
	for (int i = 0; i < 6; ++i)
	{
		std::stringstream s;
		s << "ball" << id++;

		auto ball = ballFactory->makeNew(s.str(), "sphere.mesh", Ogre::Vector3::ZERO);
		ball->getNode()->scale(0.05f, 0.05f, 0.05f);
		ball->setAlive(false);
		ball->setVelocity(Ogre::Vector3(45.0f, 45.0f, 45.0f));
	}

	/* DEBUG - SHOW BOUND BOX
	auto showBoundingBox = [&](GameObjectFactory* f) {
		auto objs = f->getAll();
		std::for_each(objs.begin(), objs.end(), [](GameObject* obj) {
			obj->getNode()->showBoundingBox(true);
		});
	};
	showBoundingBox(ballFactory);
	showBoundingBox(targetFactory);
	//*/
#pragma endregion

	fpsCtrl = new FPSController(mSceneMgr, mWindow->getViewport(0));
	timer = new Ogre::Timer();
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

	// Ball collisions.
	renderingQueuedEvents.push_back([&](Args args) {
		auto balls = ballFactory->getAll();
		auto targets = targetFactory->getAll();
		const auto playerPos = fpsCtrl->getWorldPosition();

		std::for_each(balls.begin(), balls.end(), [&](GameObject* ball) {
			if (!ball->getAlive()) {
				return;
			}

			// Check for collisions with the targets.
			std::for_each(targets.begin(), targets.end(), [&](GameObject* obj) {
				if (obj->getAlive() && ball->getAABB().intersects(obj->getAABB())) {
					obj->setAlive(false);
					ball->setAlive(false);
				}
			});

			// Kill the ball if it's too far away and still alive.
			static const float maxDist = 350.0f * 350.0f;
			if (ball->getAlive() && playerPos.squaredDistance(ball->getWorldPosition()) > maxDist) {
				ball->setAlive(false);
			}

		});
	});

	// Update FPS Controller.
	frameStartedEvents.push_back([&] (Args args) {
		if (fpsCtrl != nullptr) fpsCtrl->handleInput(*args->evt, args->key, args->mouse);
	});

	// Check for spacebar (fire event).
	renderingQueuedEvents.push_back([&](Args args) {
		if (timer->getMilliseconds() < 750) {
			return; // Limit the fire rate.
		}

		if (args->key->isKeyDown(OIS::KC_SPACE)) {
			timer->reset();
			auto ball = getNextBall();
			if (ball == nullptr) {
				return; // No balls alive.
			}
			ball->setAlive(true);
			ball->setAcceleration(fpsCtrl->getCameraRay().getDirection().normalisedCopy());
			ball->getNode()->setPosition(fpsCtrl->getCameraRay().getOrigin());
			ball->getNode()->needUpdate();
		}
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
		return b != nullptr && b->getAlive() == false; 
	});
	return result == balls.end() ? nullptr : *result;
}

} // namespace Lab4