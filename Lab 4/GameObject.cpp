#include "GameObject.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace Lab4 {
#pragma region GameObject
GameObject::GameObject(const std::string& _name, const std::string& _meshName, Ogre::SceneNode* _parentNode, Ogre::SceneManager* sceneMgr) :
	alive(true),	
	acceleration(Ogre::Vector3::ZERO),
	velocity(Ogre::Vector3::ZERO),
	name(_name),
	meshName(_meshName),
	localNode(nullptr),
	parentNode(_parentNode),
	entity(nullptr)
{
	init(sceneMgr);
	std::srand(std::time(0));
}

void GameObject::update(const Ogre::FrameEvent* evt)
{
	// Don't update dead objects.
	if (!alive) 
		return;

	localNode->translate(acceleration * velocity * evt->timeSinceLastFrame);
}

#pragma region Getters / Setters
const bool GameObject::getAlive() const
{
	return alive;
}

void GameObject::setAlive(bool b)
{
	alive = b;
	localNode->setVisible(b);
}

const Ogre::Vector3& GameObject::getAcceleration() const
{
	return acceleration;
}

const Ogre::Vector3& GameObject::getVelocity() const
{
	return velocity;
}

void GameObject::setAcceleration(const Ogre::Vector3& a)
{
	acceleration = a;
}

void GameObject::setVelocity(const Ogre::Vector3& v)
{
	velocity = v;
}

const std::string& GameObject::getName() const             
{ 
	return name; 
}

const std::string& GameObject::getMeshName() const
{ 
	return meshName; 
}

const Ogre::Vector3& GameObject::getPosition() const 
{ 
	return localNode->getPosition(); 
}
	  
Ogre::SceneNode* GameObject::getNode()  
{ 
	return localNode; 
}

const Ogre::AxisAlignedBox& GameObject::getAABB() const
{
	return entity->getWorldBoundingBox();
}
#pragma endregion

void GameObject::init(Ogre::SceneManager* sm)
{
	// Use default generated name if no name passed in.
	if (name.length() > 0)
		entity = sm->createEntity(name, meshName);
	else
		entity = sm->createEntity(meshName);

	localNode = parentNode->createChildSceneNode(name + "Node");
	localNode->attachObject(entity);

}
#pragma endregion

#pragma region GameObjectFactory
typedef GameObject TO;

GameObjectFactory::GameObjectFactory(Ogre::SceneManager* _sceneMgr, Ogre::SceneNode* _parentNode) :
	sceneMgr(_sceneMgr),
	parentNode(_parentNode)
{
	
}

GameObjectFactory::~GameObjectFactory()
{
	std::for_each(objects.begin(), objects.end(), [] (GameObject* obj) {
		if (obj != nullptr)
			delete obj;
	});
}

TO* GameObjectFactory::makeNew(const std::string& name, const std::string& mesh, const Ogre::Vector3& offset)
{
	auto obj = new TO(name, mesh, parentNode, sceneMgr);

	if (offset != Ogre::Vector3::ZERO) {
		obj->getNode()->translate(offset);
	}

	objects.push_back(obj);
	return obj;
}

TO* GameObjectFactory::get(const std::string& name)
{
	auto result = std::find_if(objects.begin(), objects.end(), [&](TO* obj) {
		return obj != nullptr && obj->getName() == name;
	});
	return result == objects.end() ? nullptr : *result;
}

std::vector<TO*>& GameObjectFactory::getAll()
{
	return objects;
}

#pragma endregion
} // namespace Lab4
