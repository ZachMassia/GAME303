#include "App.h"

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