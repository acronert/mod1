#ifndef SIMULATION_CLASS_HPP
# define SIMULATION_CLASS_HPP

#include "WaterSurface.class.hpp"
#include "Renderer.class.hpp"
#include "Camera.class.hpp"
#include "mod1.hpp"
#include <ctime>

// TEMP, until ground class implemented
#define WIDTH 100
#define HEIGHT 100
#define DISPLAY_WIDTH 1200
#define DISPLAY_HEIGHT 800

class Simulation
{
	private:
		WaterSurface*	_waterSurface;
		Renderer*		_renderer;
		Camera			_camera;
		GLFWwindow*		_window;

		s_input			_input;



	public:
		Simulation();
		~Simulation();

		void	run();
		void	init();
		void	initializeCamera();
		void	initializeGL();
};

#endif
