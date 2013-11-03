#pragma once

#include <OGRE\Ogre.h>
#include <string>
#include <vector>


namespace Lab4 {
class GameObject
{
private:
	void init(Ogre::SceneManager* sm);

	bool alive;

	Ogre::Vector3 acceleration, velocity;

	const std::string name;
	const std::string meshName;

	Ogre::SceneNode* localNode;
	Ogre::SceneNode* parentNode;
	Ogre::Entity*    entity;

public:
	GameObject(
		const std::string& _name, 
		const std::string& _meshName,
		Ogre::SceneNode*   _parentNode,
		Ogre::SceneManager* sceneMgr
	);

	void update(const Ogre::FrameEvent* evt);

#pragma region Getters / Setters
	const bool					getAlive() const;
	void						setAlive(bool b);
	const Ogre::Vector3&        getAcceleration() const;
	const Ogre::Vector3&		getVelocity() const;
	void						setAcceleration(const Ogre::Vector3& a);
	void						setVelocity(const Ogre::Vector3& v);
	const std::string&          getName() const;
	const std::string&          getMeshName() const;
	const Ogre::Vector3&        getPosition() const;
	Ogre::SceneNode*            getNode();
	const Ogre::AxisAlignedBox& getAABB() const;
#pragma endregion
};

class GameObjectFactory
{
private:
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* parentNode;
	std::vector<GameObject*> objects;

public:
	GameObjectFactory(Ogre::SceneManager* _sceneMgr, Ogre::SceneNode* _parentNode);
	~GameObjectFactory();

	GameObject* makeNew(const std::string& name, const std::string& mesh, const Ogre::Vector3& offset = Ogre::Vector3::ZERO);
	GameObject* get(const std::string& name);
	std::vector<GameObject*>& getAll();
};
} // namespace Lab4
