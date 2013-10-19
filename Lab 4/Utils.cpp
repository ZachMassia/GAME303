#include "Utils.h"

#include <OGRE/Ogre.h>


using Ogre::Vector3;
using Ogre::Math;
using Ogre::Radian;

namespace Lab4
{
void Utils::clampVector(Vector3& v, const Vector3& min, const Vector3& max)
{
	using Ogre::Math;

	v.x = Math::Clamp(v.x, min.x, max.x);
	v.y = Math::Clamp(v.y, min.y, max.y);
	v.z = Math::Clamp(v.z, min.z, max.z);
}

Vector3 Utils::yawPitchToDirVect(Radian pitch, Radian yaw)
{
	Vector3 v = Vector3::ZERO;
	v.x = Math::Sin(pitch) * Math::Cos(yaw);
	v.y = Math::Sin(pitch) * Math::Sin(yaw);
	v.z = Math::Cos(pitch);
	return v;
}
} // namespace Lab4