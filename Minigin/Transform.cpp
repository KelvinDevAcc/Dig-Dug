#include "Transform.h"

void dae::Transform::SetPosition(glm::vec3 pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;
}

void dae::Transform::SetRotation(float rotation)
{
	m_rotation = rotation;
}
