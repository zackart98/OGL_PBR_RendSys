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
