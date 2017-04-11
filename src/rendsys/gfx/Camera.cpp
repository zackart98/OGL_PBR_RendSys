#include <rendsys/gfx/Camera.hpp>
#include <rendsys/gfx/Window.hpp>


namespace rendsys
{
	// Default ctor
	Camera::Camera( )
	{
		cameraFrustum		= Frustum( );
		cameraTransform		= Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
		aspectOverride		= -1.0f;
		shouldUpdateFrustum = true;
	}



	// Get the camera world-space transform
	Transform Camera::GetTransform( )
	{
		return cameraTransform;
	}

	// Set the camera world-space transform
	void Camera::SetTransform(const Transform& newTransform)
	{
		cameraTransform		  = newTransform;
		cameraTransform.scale = glm::vec3(1, 1, 1);

		shouldUpdateFrustum = true;
	}



	// Get a view matrix for this camera
	glm::mat4 Camera::GetViewMatrix( )
	{
		UpdateFrustum( );
		return cameraFrustum.GetViewMatrix( );
	}

	// Get a projection matrix for this camera
	glm::mat4 Camera::GetProjMatrix( )
	{
		UpdateFrustum( );
		return cameraFrustum.GetProjMatrix();
	}



	// Update the frustum's values to match the camera's world-space transform
	void Camera::UpdateFrustum( )
	{
		if (shouldUpdateFrustum)
		{
			GLfloat pitch = glm::radians(cameraTransform.GetPitch( ));
			GLfloat yaw   = glm::radians(cameraTransform.GetYaw( ));
			GLfloat roll  = cameraTransform.GetRoll( );

			glm::vec3 dir;
			dir.x = glm::cos(pitch) * glm::cos(yaw);
			dir.y = glm::sin(pitch);
			dir.z = glm::cos(pitch) * glm::sin(yaw);
			dir   = glm::normalize(dir);

			cameraFrustum.SetViewDir(dir);
			cameraFrustum.SetRoll(roll);

			cameraFrustum.SetViewPos(cameraTransform.translate);

			if (aspectOverride < 0.0f)
			{
				glm::vec2 fbSz(Window::Inst( ).FramebufferSize( ));
				GLfloat   aspectTmp = (fbSz.x / fbSz.y);
				cameraFrustum.SetAspect(aspectTmp);
			}
			else
			{
				cameraFrustum.SetAspect(aspectOverride);
			}

			shouldUpdateFrustum = false;
		}
	}
}
