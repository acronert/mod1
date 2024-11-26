#include "WaterSurface.class.hpp"

WaterSurface::WaterSurface(int sizeX, int sizeY)
	: _sizeX(sizeX), _sizeY(sizeY)
{
	// can cause problems when size is large (> 1 million)
	_cell.resize(_sizeX * _sizeY);

	for (int y = 0; y < _sizeY; y++) {
		for (int x = 0; x < _sizeX; x++) {
			Cell* Ncell = (y < _sizeY - 1) ? &_cell[index(x, y + 1)] : nullptr;
			Cell* Ecell = (x < _sizeX - 1) ? &_cell[index(x + 1, y)] : nullptr;
			Cell* Scell = (y > 0) ? &_cell[index(x, y - 1)] : nullptr;
			Cell* Wcell = (x > 0) ? &_cell[index(x - 1, y)] : nullptr;

			_cell[index(x, y)] = Cell(10.0f, 0.0f, Ncell, Ecell, Scell, Wcell);
		}
	}
}

WaterSurface::~WaterSurface() {}

WaterSurface::WaterSurface(const WaterSurface& other)
	: _sizeX(other._sizeX), _sizeY(other._sizeY), _cell(other._cell)
{}

WaterSurface& WaterSurface::operator=(const WaterSurface& other) {
	if (this != &other) {
		_sizeX = other._sizeX;
		_sizeY = other._sizeY;

	}
	return *this;
}

int WaterSurface::index(int x, int y) {
	return (x + y * _sizeX);
}

// Check the underflow in a second loop in case of problems (but its slower)
void	WaterSurface::update() {
	for (unsigned long int i = 0; i < _cell.size(); i++) {
		_cell[i].updateVelocity();
		_cell[i].updateWaterLevel();
		_cell[i].updateNormal();
		if (_cell[i].getWaterLevel() < 0) {
			checkUnderflow(i % _sizeX, i / _sizeY);
		}
	}
}

void	WaterSurface::checkUnderflow(int x, int y) {
	if (x < 0 || y < 0 || x >= _sizeX || y >= _sizeY || _cell[index(x, y)].getWaterLevel() >= 0)
		return;

	_cell[index(x,y)].resolveUnderflow();

	checkUnderflow(x-1, y);
	checkUnderflow(x, y-1);
	// the x+1 and y+1 are not necessary, cause they will be met next in the update loop
	checkUnderflow(x+1, y);
	checkUnderflow(x, y+1);
}

void	WaterSurface::setGroundLevel(int x, int y, float h) {
	if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY)
		return;
	_cell[index(x, y)].setGroundLevel(h);
}


void	WaterSurface::setWaterLevel(int x, int y, float h) {
	if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY)
		return;
	_cell[index(x, y)].setWaterLevel(h);
}

float	WaterSurface::getTotalWaterLevel() {
	float sum = 0;

	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			sum += _cell[index(x, y)].getWaterLevel();
		}
	}
	return sum;
}

std::vector<Cell>&	WaterSurface::getCells() {
	return	_cell;
}

int	WaterSurface::getSizeX() { return _sizeX; }

int	WaterSurface::getSizeY() { return _sizeY; }

void	WaterSurface::displayASCII() {
	float	min = 0.0f;
	float	max = 2.0f;
	int		color;
	float	delta = (max - min) / 24.0f;

	for (int y = _sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < _sizeX; x++) {
			color = floor(_cell[index(x, y)].getWaterLevel() / delta);
			if (color <= 0)
				color = -36;
			else if (color > 23)
				color = 23;
			std::cout << "\033[48;5;" << color + 232 << "m" << "  " << "\033[0m";
		}
		std::cout << std::endl;
	}
	std::cout << "Wave speed = " << WAVE_SPEED <<std::endl;
}

float	WaterSurface::getWaterVertexHeight(int x, int y) {
	return _cell[index(x, y)].getWaterVertexHeight();
}

void	WaterSurface::loadGroundMap(const std::vector<float>& heightMap) {
	for (int y = 0; y < _sizeY; y++) {
		for (int x = 0; x < _sizeX; x++) {
			int idx = index(x, y);
			if (static_cast<unsigned long>(idx) >= heightMap.size())
				break;
			_cell[index(x, y)].setGroundLevel(heightMap[idx]);
		}
	}
}

void	WaterSurface::resetWater() {
	for (int y = 0; y < _sizeY; y++) {
		for (int x = 0; x < _sizeX; x++) {
			_cell[index(x, y)].resetWater();
		}
	}
}

void	WaterSurface::riseWater(float intensity, float threshold) {
	if (intensity <= 0)
		return;

	for (int y = 0; y < _sizeY; y++) {
		for (int x = 0; x < _sizeX; x++) {
			int idx = index(x, y);
			if (_cell[idx].getGroundLevel() <= threshold)
				_cell[idx].addWater(intensity);
		}
	}
}

// rainIntensity : at 0 : no rain, at 1 : SIZE * SIZE droplets per tick
void	WaterSurface::makeRain(float rainIntensity, float dropletSize) {
	if (rainIntensity <= 0 || dropletSize <= 0)
		return;

	int	n_droplets = rainIntensity * _sizeX * _sizeY;

	while (n_droplets--){
		int x = rand() % _sizeX;
		int y = rand() % _sizeY;

		_cell[index(x, y)].addWater(dropletSize);
	}
}

void	WaterSurface::makeWave(float intensity) {
	if (intensity <= 0)
		return;

	for (int x = 0; x < _sizeX; x++) {
		_cell[index(x, 0)].addWater(intensity);
	}
}

void	WaterSurface::updateGroundNormal() {
	for (int y = 0; y < _sizeY; y++) {
		for (int x = 0; x < _sizeX; x++) {
			_cell[index(x, y)].updateGroundNormal();
		}
	}
}