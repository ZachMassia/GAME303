#pragma once

#include <string>

#pragma region Forward Declerations
namespace Ogre {
	class Vector3;
	class Radian;
	class Ray;
}
#pragma region

namespace Lab4 {
class Utils
{
private:
	Utils() {}

public:
	static void clampVector(Ogre::Vector3& v, const Ogre::Vector3& min, const Ogre::Vector3& max);

	static Ogre::Vector3 yawPitchToDirVect(Ogre::Radian pitch, Ogre::Radian yaw);

	static const std::string rayToStr(const Ogre::Ray& ray);
};
} // namespace Lab4

