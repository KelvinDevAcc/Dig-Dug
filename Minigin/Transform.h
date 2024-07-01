#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		const float& GetRotation() const { return m_rotation; }

		void SetPosition(glm::vec3 pos);
		void SetRotation(float rotation);

	private:
		glm::vec3 m_position = {};
		float m_rotation = 0;
	};
}
