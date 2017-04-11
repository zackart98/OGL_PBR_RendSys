#ifndef RENDSYS_CAMERA_HPP_INC
#define RENDSYS_CAMERA_HPP_INC

#include <rendsys/gfx/Frustum.hpp>
#include <rendsys/math/Transform.hpp>

#include <glm/glm.hpp>

namespace rendsys
{
	// Simulates a camera, with a world-space transformation and a frustum
	class Camera
	{
	  public:
		// Default ctor
		Camera( );

		
		
		
		// Get the camera world-space transform
		Transform GetTransform();
		
		// Set the camera world-space transform
		void SetTransform(const Transform& newTransform);
		
		
		// Get a view matrix for this camera
		glm::mat4 GetViewMatrix();
		
		// Get a projection matrix for this camera
		glm::mat4 GetProjMatrix();
		
		
	  private:
		// The camera's viewing frustum
		Frustum cameraFrustum;

		// The camera world-space transform
		Transform cameraTransform;
		
			
		// Override for the aspect ratio: set to < 0 for automatic
		GLfloat aspectOverride;
		
		

		// Frustum is out of date?
		bool shouldUpdateFrustum;
		
		// Update the frustum's values to match the camera's world-space transform
		void UpdateFrustum();
		
	};
}

#endif
