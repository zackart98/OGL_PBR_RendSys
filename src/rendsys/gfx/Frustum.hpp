#ifndef RENDSYS_FRUSTUM_HPP_INC
#define RENDSYS_FRUSTUM_HPP_INC

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace rendsys
{
	// A perspective viewing frustum
	class Frustum
	{
	  public:
		// Indices of each frustum plane in the array returned by 'FrustumPlanes()'
		static constexpr GLuint NEAR_IDX   = 0;
		static constexpr GLuint FAR_IDX	= 1;
		static constexpr GLuint TOP_IDX	= 2;
		static constexpr GLuint BOTTOM_IDX = 3;
		static constexpr GLuint LEFT_IDX   = 4;
		static constexpr GLuint RIGHT_IDX  = 5;

		// Default ctor
		Frustum( );

		// Get a view matrix for this frustum
		glm::mat4 GetViewMatrix( );

		// Get a projection matrix for this frustum
		glm::mat4 GetProjMatrix( );

		
		// Get the field of view
		GLfloat GetFOV();
		
		// Set the field of view
		void SetFOV(GLfloat newFOV);
		
		
		// Get the aspect ratio
		GLfloat GetAspect();
		
		// Set the field of view
		void SetAspect(GLfloat newAspect);
		
		
		// Get the near clipping plane distance
		GLfloat GetZNear();
		
		// Set the near clipping plane distance
		void SetZNear(GLfloat newZNear);
		
		
		// Get the far clipping plane distance
		GLfloat GetZFar();
		
		// Set the far clipping plane distance
		void SetZFar(GLfloat newZFar);
		
		
		// Get the view position
		glm::vec3 GetViewPos();
		
		// Set the view position
		void SetViewPos(const glm::vec3& newViewPos);
		
		
		// Get the view direction
		glm::vec3 GetViewDir();
		
		// Set the view direction
		void SetViewDir(const glm::vec3& newViewDir);
		
		
		
		// Get the rotation on the Z-Axis
		GLfloat GetRoll();
		
		// Set the rotation on the Z-Axis
		void SetRoll(GLfloat newRoll);
		
	  private:
		// Field-of-view, in degrees
		GLfloat fov;

		// Aspect ratio: Width / Height
		GLfloat aspect;

		// Near clipping plane distance
		GLfloat zNear;

		// Far clipping plane distance
		GLfloat zFar;


		// Eye position
		glm::vec3 viewPos;

		// View direction
		glm::vec3 viewDir;

		// Rotation on the Z-Axis
		GLfloat rollAngle;


		// Frustum planes out of date?
		bool shouldUpdatePlanes;


		// View matrix out of date?
		bool shouldUpdateViewMat;
		// The view matrix
		glm::mat4 viewMat;

		// Projection matrix out of date?
		bool shouldUpdateProjMat;
		// The projection matrix
		glm::mat4 projMat;
	};
}

#endif
