#include <rendsys/gfx/Frustum.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace rendsys
{

	// Default ctor
	Frustum::Frustum( )
	{
		fov	= 45.0f;
		aspect = 1.0f;
		zNear  = 0.1f;
		zFar   = 100.0f;

		viewPos   = glm::vec3(0, 0, 0);
		viewDir   = glm::vec3(0, 0, 1);
		rollAngle = 0.0f;

		viewMat = glm::mat4(1.0f);
		projMat = glm::mat4(1.0f);

		shouldUpdateViewMat = true;
		shouldUpdateProjMat = true;
	}

	// Get a view matrix for this frustum
	glm::mat4 Frustum::GetViewMatrix( )
	{
		if (shouldUpdateViewMat)
		{
			glm::vec3 tmpDir  = glm::normalize(tmpDir);
			glm::vec3 worldUp = glm::vec3(0, 1, 0);
			glm::vec3 up =
				(glm::abs(glm::dot(worldUp, tmpDir)) >= 0.95f) ? (glm::vec3(0, 0, 1)) : (worldUp);

			viewMat = glm::lookAt(viewPos, viewPos + tmpDir, up);
			if (rollAngle != 0.0f)
			{
				glm::mat4 rollMat = glm::rotate(glm::mat4(), glm::radians(rollAngle), glm::vec3(0, 0, 1));
				viewMat = rollMat * viewMat;
			}
		}
		return viewMat;
	}
}
