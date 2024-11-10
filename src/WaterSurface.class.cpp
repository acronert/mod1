#include "WaterSurface.class.hpp"

WaterSurface::WaterSurface(int sizeX, int sizeY)
	: _sizeX(sizeX), _sizeY(sizeY), _cellSize(0.001f),
	_waveSpeed(2.0f), _dt(1.0f / 10000.0f), _halfLife(0.07f)
{
	// _waveSpeed = std::min(WAVE_SPEED, static_cast<float>(0.5 * _cellSize / DELTA_TIME));
	_k = (_waveSpeed * _waveSpeed) / (_cellSize * _cellSize);
	_cell.resize(_sizeX * _sizeY);

}

WaterSurface::~WaterSurface() {}

WaterSurface::WaterSurface(const WaterSurface& other)
	: _sizeX(other._sizeX), _sizeY(other._sizeY), _cellSize(other._cellSize),
	_waveSpeed(other._waveSpeed), _k(other._k), _dt(other._dt), _halfLife(other._halfLife), _cell(other._cell)
{}

WaterSurface& WaterSurface::operator=(const WaterSurface& other) {
	if (this != &other) {
		_sizeX = other._sizeX;
		_sizeY = other._sizeY;
		_cellSize = other._cellSize;
		_waveSpeed = other._waveSpeed;
		_k = other._k;
		_halfLife = other._halfLife;
		_dt = other._dt;
		_cell = other._cell;
	}
	return *this;
}

int WaterSurface::index(int x, int y) {
	return (x + y * _sizeX);
}

void	WaterSurface::setCellHeight(int x, int y, int h) {
	if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY)
		return ;
	_cell[index(x, y)].h = h;
}


float	WaterSurface::acceleration(Cell& target, int neighborX, int neighborY) {
	if (neighborX < 0 || neighborX >= _sizeX || neighborY < 0 || neighborY >= _sizeY)
		return 0;
	Cell& neighbor = _cell[index(neighborX, neighborY)];
	return _k * (neighbor.h - target.h);
}

void	WaterSurface::update() {
	float damp = pow(0.3, _dt / _halfLife); // damp 0 for more smoothing

	// get acceleration
	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			Cell& target = _cell[index(x, y)];
			target.a = (
				acceleration(target, x+1, y) +
				acceleration(target, x-1, y) +
				acceleration(target, x, y+1) +
				acceleration(target, x, y-1)
			);
		}
	}

	// set heigh and velocity
	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			Cell& target = _cell[index(x, y)];
			target.v = (damp * target.v) + (_dt * target.a);
			target.h += (_dt * target.v);
		}
	}
}

void	WaterSurface::displayHeight() {
	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			float height = _cell[index(x, y)].h;
			std::cout << height << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

float	WaterSurface::getSumHeight() {
	float sum = 0;
	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			sum += _cell[index(x, y)].h;
		}
	}
	return sum;
}

std::vector<Cell>	WaterSurface::getCells(){
	return _cell;
}
