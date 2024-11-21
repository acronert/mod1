#include "Camera.class.hpp"

Camera::Camera() : posX(0.0f), posY(0.0f), posZ(0.0f),
					yaw(0.0f), pitch(0.0f) {}

Camera::~Camera() {}

Camera::Camera(const Camera& other)
{
	posX = other.posX;
	posY = other.posY;
	posZ = other.posZ;
	yaw = other.yaw;
	pitch = other.pitch;
}

Camera& Camera::operator=(const Camera& other)
{
	if (this != &other)
	{
		posX = other.posX;
		posY = other.posY;
		posZ = other.posZ;
		yaw = other.yaw;
		pitch = other.pitch;
	}
	return (*this);
}

void	Camera::update(const s_input& input) {
	float yawRad = yaw * DEG_TO_RAD;

	float forwardX = cos(yawRad);
	float forwardY = sin(yawRad);
	float forwardZ = 0;

	float rightX = -sin(yawRad);
	float rightY = cos(yawRad);
	float rightZ = 0;

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

	yaw += input.yawLeft ? CAMERA_ROT_SPEED : 0;
	yaw += input.yawRight ? -CAMERA_ROT_SPEED : 0;
	pitch += input.pitchUp ? CAMERA_ROT_SPEED : 0;
	pitch += input.pitchDown ? -CAMERA_ROT_SPEED : 0;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	if (yaw > 360.0f)
		yaw -= 360.0f;
	if (yaw < 0.0f)
		yaw += 360.0f;
}