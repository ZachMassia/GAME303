#pragma once

#include <OGRE\Ogre.h>
#include <string>
#include <vector>


namespace Lab4 {
class GameObject
{
private:
	// Creates the entity using the mesh name, and optionally, 
	// the node name.
	// The entity is attached to it's own node which is a child of the
	// parent node passed to the constructor.
	void init(Ogre::SceneManager* sm);

	// If false, the object is not updated or rendered.
	bool alive;

	Ogre::Vector3 acceleration, velocity;

	// A ray which is cast in the direction the object is moving.
	Ogre::Ray ray;

	// The object's name. If not empty, the object's local node will be
	// named `{name}Node`.
	const std::string name;

	// The mesh file used for the entity.
	const std::string meshName;

	// The object's entity is attached to this node.
	Ogre::SceneNode* localNode;

	// Parent to the local node.
	Ogre::SceneNode* parentNode;

	// The object's entity.
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
	// The object's world bounding box.
	const Ogre::AxisAlignedBox& getAABB() const;
	// A ray cast in the objects moving direction.
	const Ogre::Ray&			getRay() const;
	// The object's world coordinates.
	const Ogre::Vector3&		getWorldPosition() const;
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

	// Constructs a new GameObject. The new object is returned. 
	// The factory has ownership of the object, and takes care of deleting
	// the memory for all GameObjects.
	GameObject* makeNew(const std::string& name, const std::string& mesh, const Ogre::Vector3& offset = Ogre::Vector3::ZERO);
	
	// Returns an object. If no objects match the name, nullptr is returned.
	GameObject* get(const std::string& name);

	// Returns a reference to the internal object vector. Useful for iteration over
	// all objects.
	std::vector<GameObject*>& getAll();
};
} // namespace Lab4
