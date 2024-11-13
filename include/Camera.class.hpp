#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP

# include "Input.struct.hpp"
# include <cmath>

# define CAMERA_MOV_SPEED	0.3f
# define CAMERA_ROT_SPEED	0.7f
# define DEG_TO_RAD			M_PI / 180.0f

class Camera
{
	private:

	public:
		float	posX;
		float	posY;
		float	posZ;
		// float	targetX;
		// float	targetY;
		// float	targetZ;

		float	yaw;
		float	pitch;

		Camera();
		~Camera();
		Camera(const Camera& other);
		Camera& operator=(const Camera& other);

		void	update(const s_input& input);
};

#endif
