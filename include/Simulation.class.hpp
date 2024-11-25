#ifndef SIMULATION_CLASS_HPP
# define SIMULATION_CLASS_HPP

#include "WaterSurface.class.hpp"
#include "Renderer.class.hpp"
#include "Camera.class.hpp"
#include "mod1.hpp"
#include <ctime>
#include <algorithm>

// TEMP, until ground class implemented
#define DISPLAY_WIDTH 1200
#define DISPLAY_HEIGHT 800

#define TARGET_FPS 30

class Simulation
{
	private:
		WaterSurface*	_waterSurface;
		Renderer*		_renderer;
		GLFWwindow*		_window;
		Camera			_camera;
		s_input			_input;

		int				_rain_intensity;
		int				_rise_intensity;
		int				_wave_intensity;

	public:
		Simulation();
		~Simulation();

		void	run(std::vector<float> heightMap, int size);
		void	initializeWaterSurface(std::vector<float> heightMap, int size);
		void	initializeCamera(int size);
		void	waterControl();
};

#endif
