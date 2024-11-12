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

struct s_vertex {
	float x;
	float y;
	float z;
};

class Renderer
{
	private:
		GLFWwindow*				_window;
		int						_display_width;
		int						_display_height;

		// s_vertex				_cameraPos;
		// float				_cameraPitch;
		// float				_cameraYaw;


		std::vector<s_vertex>	_waterVertices;
		std::vector<s_vertex>	_groundVertices;
		int						_sizeX;
		int						_sizeY;

	public:
		Renderer();
		~Renderer();
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer& other);

		void	init(int width, int height, std::string title);
		void	render(WaterSurface& surface);
		void	generateVertices(std::vector<Cell>& cells, int sizeX, int sizeY);
		void	drawWaterVertices();
		void	drawGroundVertices();
		int		index(int x, int y);


};

#endif