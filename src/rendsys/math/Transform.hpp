#ifndef RENDSYS_TRANSFORM_HPP_INC
#define RENDSYS_TRANSFORM_HPP_INC

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace rendsys
{

	// A 3D transform with translation, rotation, and scale components
	struct Transform
	{
		// The translation component of the transformation
		glm::vec3 translate;

		// The rotation component of the transformation: In quaternion form
		glm::fquat rotate;

		// The scale component of the transformation
		glm::vec3 scale;



		// Default ctor
		Transform( )
			: translate(0.0f, 0.0f, 0.0f)
			, rotate(glm::vec3(0.0f, 0.0f, 0.0f))
			, scale(1.0f, 1.0f, 1.0f)
		{
		}

		// Construct from transformation components
		// Scale defaults to 1.0
		Transform(glm::vec3 t, glm::fquat r, glm::vec3 s = glm::vec3(1.0f))
			: translate(t)
			, rotate(r)
			, scale(s)
		{
		}

		// Construct from transformation components, with Euler angles for rotation
		// Scale defaults to 1.0
		Transform(glm::vec3 t, glm::vec3 eulerAngles, glm::vec3 s = glm::vec3(1.0f))
			: translate(t)
			, rotate(glm::radians(eulerAngles))
			, scale(s)
		{
		}

		// Get the rotation pitch component (Rotation on X-axis) in degrees
		float GetPitch( )
		{
			return glm::mod(glm::degrees(glm::pitch(rotate)), 360.0f);
		}

		// Get the rotation yaw component (Rotation on Y-axis) in degrees
		float GetYaw( )
		{
			return glm::mod(glm::degrees(glm::yaw(rotate)), 360.0f);
		}

		// Get the rotation roll component (Rotation on Z-axis) in degrees
		float GetRoll( )
		{
			return glm::mod(glm::degrees(glm::roll(rotate)), 360.0f);
		}

		// Set the rotation pitch component (Rotation on X-axis) in degrees
		void SetPitch(float pitch)
		{
			pitch = glm::mod(pitch, 360.0f);

			float yaw  = ClampQuatEuler(glm::degrees(glm::yaw(rotate)));
			float roll = GetRoll( );

			rotate = glm::fquat(glm::radians(glm::vec3(pitch, yaw, roll)));
		}

		// Set the rotation yaw component (Rotation on Y-axis) in degrees
		void SetYaw(float yaw)
		{
			yaw = ClampQuatEuler(yaw);

			float pitch = GetPitch( );
			float roll  = GetRoll( );

			rotate = glm::fquat(glm::radians(glm::vec3(pitch, yaw, roll)));
		}

		// Set the rotation roll component (Rotation on Z-axis) in degrees
		void SetRoll(float roll)
		{
			roll = glm::mod(roll, 360.0f);

			float pitch = GetPitch( );
			float yaw   = ClampQuatEuler(glm::degrees(glm::yaw(rotate)));

			rotate = glm::fquat(glm::radians(glm::vec3(pitch, yaw, roll)));
		}


		// Get a transformation matrix matching this structure
		glm::mat4 GetMatrix( )
		{
			glm::mat4 rotMat  = glm::mat4_cast(rotate);
			glm::mat4 tranMat = glm::translate(glm::mat4( ), translate);
			glm::mat4 sclMat  = glm::scale(glm::mat4( ), scale);

			return tranMat * rotMat * sclMat;
		}

		// Get a look-at matrix that matches this structure
		glm::mat4 GetLookAtMatrix( )
		{
			glm::mat4 rotMat   = glm::mat4_cast(rotate);
			glm::mat4 transMat = glm::translate(glm::mat4( ), -translate);

			return rotMat * transMat;
		}

	  private:
		static float ClampQuatEuler(float a)
		{
			a = glm::mod(a, 360.0f);
			a = (a > 90.0f && a < 270.0f) ? (-a) : (a);
			return a;
		}
	};
}

#endif
