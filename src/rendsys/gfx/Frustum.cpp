#include <rendsys/gfx/Frustum.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
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

		shouldUpdatePlanes = true;
	}

	// Get a view matrix for this frustum
	glm::mat4 Frustum::GetViewMatrix( )
	{
		if (shouldUpdateViewMat)
		{
			glm::vec3 tmpDir = glm::normalize(viewDir);

			glm::vec3 worldUp = glm::vec3(0, 1, 0);
			glm::vec3 up =
				(glm::abs(glm::dot(worldUp, tmpDir)) >= 0.95f) ? (glm::vec3(0, 0, 1)) : (worldUp);

			viewMat = glm::lookAt(viewPos, viewPos + tmpDir, worldUp);
			if (rollAngle != 0.0f)
			{
				glm::mat4 rollMat =
					glm::rotate(glm::mat4( ), glm::radians(rollAngle), glm::vec3(0, 0, 1));
				viewMat = rollMat * viewMat;
			}
			shouldUpdateViewMat = false;
		}
		return viewMat;
	}

	// Get a projection matrix for this frustum
	glm::mat4 Frustum::GetProjMatrix( )
	{
		if (shouldUpdateProjMat)
		{
			projMat				= glm::perspective(glm::radians(fov), aspect, zNear, zFar);
			shouldUpdateProjMat = false;
		}

		return projMat;
	}



	// Get the field of view
	GLfloat Frustum::GetFOV( )
	{
		return fov;
	}

	// Set the field of view
	void Frustum::SetFOV(GLfloat newFOV)
	{
		if (newFOV >= 0.0f)
		{
			fov					= newFOV;
			shouldUpdateProjMat = true;
			shouldUpdatePlanes  = true;
		}
	}


	// Get the aspect ratio
	GLfloat Frustum::GetAspect( )
	{
		return aspect;
	}

	// Set the field of view
	void Frustum::SetAspect(GLfloat newAspect)
	{
		if (newAspect > 0.0f)
		{
			aspect				= newAspect;
			shouldUpdateProjMat = true;
			shouldUpdatePlanes  = true;
		}
	}

	// Get the near clipping plane distance
	GLfloat Frustum::GetZNear( )
	{
		return zNear;
	}

	// Set the near clipping plane distance
	void Frustum::SetZNear(GLfloat newZNear)
	{
		if (newZNear >= 0.0f)
		{
			zNear				= newZNear;
			shouldUpdateProjMat = true;
			shouldUpdatePlanes  = true;
		}
	}

	// Get the near clipping plane distance
	GLfloat Frustum::GetZFar( )
	{
		return zNear;
	}

	// Set the near clipping plane distance
	void Frustum::SetZFar(GLfloat newZFar)
	{
		if (newZFar >= 0.0f)
		{
			zFar				= newZFar;
			shouldUpdateProjMat = true;
			shouldUpdatePlanes  = true;
		}
	}

	// Get the view position
	glm::vec3 Frustum::GetViewPos( )
	{
		return viewPos;
	}

	// Set the view position
	void Frustum::SetViewPos(const glm::vec3& newViewPos)
	{
		viewPos				= newViewPos;
		shouldUpdateViewMat = true;
		shouldUpdatePlanes  = true;
	}


	// Get the view direction
	glm::vec3 Frustum::GetViewDir( )
	{
		return viewPos;
	}

	// Set the view direction
	void Frustum::SetViewDir(const glm::vec3& newViewDir)
	{
		{
			viewDir				= newViewDir;
			shouldUpdateViewMat = true;
			shouldUpdatePlanes  = true;
		}
	}



	// Get the rotation on the Z-Axis
	GLfloat Frustum::GetRoll( )
	{
		return rollAngle;
	}

	// Set the rotation on the Z-Axis
	void Frustum::SetRoll(GLfloat newRoll)
	{
		rollAngle			= newRoll;
		shouldUpdateViewMat = true;
		shouldUpdatePlanes  = true;
	}
}
