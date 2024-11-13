#include "Simulation.class.hpp"

Simulation::Simulation()
{
	_waterSurface = new WaterSurface(WIDTH, HEIGHT);
	_renderer = new Renderer();
}

Simulation::~Simulation()
{
	delete _waterSurface;
	delete _renderer;
}

Simulation::Simulation(const Simulation& other)
{
	// _value = other.value;
	// _ptr = new int;
	// *_ptr = *source._ptr;
	(void)other;
}

Simulation& Simulation::operator=(const Simulation& other)
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

void	Simulation::init() {
	// parse and create the ground map
	for (int i = HEIGHT / 5; i < 4 * HEIGHT / 5; i++) {
		_waterSurface->setGroundLevel(WIDTH/3 -2, i, 3.0f);
		_waterSurface->setGroundLevel(WIDTH/3 -1, i, 3.5f);
		_waterSurface->setGroundLevel(WIDTH/3, i, 4.0f);
		_waterSurface->setGroundLevel(WIDTH/3 + 1, i, 3.5f);
		_waterSurface->setGroundLevel(WIDTH/3 + 2, i, 3.0f);
	}
	for (int i = 0; i < WIDTH / 5; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			_waterSurface->setGroundLevel(i, j, (WIDTH / 5) - i);
		}
	}
	// create the water surface / set the type of water event (rain, rise, wave)
	_waterSurface->setWaterLevel(WIDTH / 2, HEIGHT / 2, 100);
}

void	Simulation::run() {

	// Initialize
	init();

	_renderer->render(*_waterSurface);

}