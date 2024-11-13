#ifndef RENDERER_CLASS_HPP
# define RENDERER_CLASS_HPP

#include <GL/gl.h>
#include <GL/glu.h>		// For perspective projection
#include <GLFW/glfw3.h>	// For window and context management
#include <math.h>

#include <iostream>
#include <vector>

#include "WaterSurface.class.hpp"

#define DISPLAY_WIDTH	1200
#define DISPLAY_HEIGHT	800
#define CAMERA_SPEED	0.1f


struct Vertex {
	float x;
	float y;
	float z;
};

struct Camera {
	float posX;
	float posY;
	float posZ;
	float targetX;
	float targetY;
	float targetZ;
};

struct Controls {
	bool forward = false;
	bool backward = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};

class Renderer
{
	private:
		GLFWwindow*				_window;
		Camera					_camera;
		Controls				_controls;
		int						_display_width;
		int						_display_height;
		std::vector<Vertex>		_waterVertices;
		std::vector<Vertex>		_groundVertices;
		int						_sizeX;
		int						_sizeY;

		int		index(int x, int y);
		void	initializeGL();
		void	setupCamera();
		void	initCamera();

	public:
		Renderer();
		~Renderer();
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer& other);

		void	render(WaterSurface& surface);
		void	generateVertices(std::vector<Cell>& cells);
		void	drawWaterVertices();
		void	drawGroundVertices();

};

#endif