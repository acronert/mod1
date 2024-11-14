#include "Camera.class.hpp"

Camera::Camera() : posX(0.0f), posY(0.0f), posZ(0.0f),
					yaw(0.0f), pitch(0.0f)
{

}

Camera::~Camera()
{

}

Camera::Camera(const Camera& other)
{
	// _value = other.value;
	// _ptr = new int;
	// *_ptr = *source._ptr;
	(void)other;
}

Camera& Camera::operator=(const Camera& other)
{
	if (this != &other)
	{
		// _value = other.value;

		// delete  _ptr;
		// _ptr = new int;
		// *_ptr = *source._ptr;
	}
	return (*this);
}

void	Camera::update(const s_input& input) {
	// Calculate the forward direction based on yaw and pitch
	float yawRad = yaw * DEG_TO_RAD;
	// float pitchRad = pitch * DEG_TO_RAD;

	// // Forward vector
	// float forwardX = cos(pitchRad) * cos(yawRad);
	// float forwardY = cos(pitchRad) * sin(yawRad);
	// float forwardZ = sin(pitchRad);

	// Forward vector
	float forwardX = cos(yawRad);
	float forwardY = sin(yawRad);
	float forwardZ = 0;

	// Right vector (perpendicular to forward, no vertical component)
	float rightX = -sin(yawRad);
	float rightY = cos(yawRad);
	float rightZ = 0;

	// Apply movement in each direction
	if (input.forward) {
		posX += forwardX * CAMERA_MOV_SPEED;
		posY += forwardY * CAMERA_MOV_SPEED;
		posZ += forwardZ * CAMERA_MOV_SPEED;
	}
	if (input.backward) {
		posX -= forwardX * CAMERA_MOV_SPEED;
		posY -= forwardY * CAMERA_MOV_SPEED;
		posZ -= forwardZ * CAMERA_MOV_SPEED;
	}
	if (input.left) {
		posX += rightX * CAMERA_MOV_SPEED;
		posY += rightY * CAMERA_MOV_SPEED;
		posZ += rightZ * CAMERA_MOV_SPEED;
	}
	if (input.right) {
		posX -= rightX * CAMERA_MOV_SPEED;
		posY -= rightY * CAMERA_MOV_SPEED;
		posZ -= rightZ * CAMERA_MOV_SPEED;
	}
	if (input.up) {
		posZ += CAMERA_MOV_SPEED;
	}
	if (input.down) {
		posZ -= CAMERA_MOV_SPEED;
	}

	// Update yaw and pitch
	yaw += input.yawLeft ? CAMERA_ROT_SPEED : 0;
	yaw += input.yawRight ? -CAMERA_ROT_SPEED : 0;
	pitch += input.pitchUp ? CAMERA_ROT_SPEED : 0;
	pitch += input.pitchDown ? -CAMERA_ROT_SPEED : 0;

	// Constrain pitch to avoid flipping the camera
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Wrap yaw to keep it within [0, 360]
	if (yaw > 360.0f)
		yaw -= 360.0f;
	if (yaw < 0.0f)
		yaw += 360.0f;
}