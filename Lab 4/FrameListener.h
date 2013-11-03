#pragma once

#include "FPSController.h"
#include "GameObject.h"
#include "Event.h"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>


namespace Lab4 {
class FrameListener : public Ogre::FrameListener
{
private:
	struct _FrameEventArgs
	{
		OIS::Keyboard*          key;
		OIS::Mouse*             mouse;
		const Ogre::FrameEvent* evt;

		_FrameEventArgs(OIS::Keyboard*          _key   = nullptr, 
						OIS::Mouse*             _mouse = nullptr, 
						const Ogre::FrameEvent* _evt   = nullptr) :
			key(_key), mouse(_mouse), evt(_evt)
		{
		}
	};

	Ogre::SceneManager* sceneMgr;
	Ogre::Camera*       camera;
	
	OIS::InputManager* man;
	OIS::Keyboard*     key;
	OIS::Mouse*        mouse;

	FPSController* fpsCtrl;
	GameObjectFactory* targetFactory;

public:
	FrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* _sceneMgr, FPSController* _fpsCtrl, GameObjectFactory* _targetfactory);
	~FrameListener();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	typedef _FrameEventArgs* FrameEventArgs;

	Event<_FrameEventArgs*> OnFrameStarted;
	Event<_FrameEventArgs*> OnFrameRenderingQueued;
};
} // namespace Lab4
