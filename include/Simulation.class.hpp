#ifndef SIMULATION_CLASS_HPP
# define SIMULATION_CLASS_HPP

#include "WaterSurface.class.hpp"
#include "Renderer.class.hpp"
#include <ctime>

// TEMP, until ground class implemented
#define WIDTH 100
#define HEIGHT 100

class Simulation
{
	private:
		WaterSurface*	_waterSurface;
		Renderer*		_renderer;

	public:
		Simulation();
		~Simulation();
		Simulation(const Simulation& other);
		Simulation& operator=(const Simulation& other);

		void	run();
		void	init();
};

#endif
