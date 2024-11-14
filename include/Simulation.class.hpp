#ifndef SIMULATION_CLASS_HPP
# define SIMULATION_CLASS_HPP

#include "WaterSurface.class.hpp"
#include "Renderer.class.hpp"
#include "Camera.class.hpp"
#include "mod1.hpp"
#include <ctime>

// TEMP, until ground class implemented
#define DISPLAY_WIDTH 1200
#define DISPLAY_HEIGHT 800

#define TARGET_FPS 60

class Simulation
{
	private:
		WaterSurface*	_waterSurface;
		Renderer*		_renderer;
		Camera			_camera;
		GLFWwindow*		_window;
		s_input			_input;

		size_t			_size;

	public:
		Simulation();
		~Simulation();

		void	run(std::vector<float> heightMap, int size);

		void	initializeWaterSurface(std::vector<float> heightMap);
		void	initializeCamera(int size);
		void	initializeGL();

		void	waterControl();
};

#endif
