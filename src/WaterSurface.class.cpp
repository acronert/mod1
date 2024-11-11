#include "WaterSurface.class.hpp"

WaterSurface::WaterSurface(int sizeX, int sizeY)
	: _sizeX(sizeX), _sizeY(sizeY)
{
	_cell.resize(_sizeX * _sizeY);

	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeY; y++) {
			Cell* Ncell = (y < _sizeY - 1) ? &_cell[index(x, y + 1)] : nullptr;
			Cell* Ecell = (x < _sizeX - 1) ? &_cell[index(x + 1, y)] : nullptr;
			Cell* Scell = (y > 0) ? &_cell[index(x, y - 1)] : nullptr;
			Cell* Wcell = (x > 0) ? &_cell[index(x - 1, y)] : nullptr;
			_cell[index(x, y)] = Cell(0.0f, Ncell, Ecell, Scell, Wcell);
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

void	WaterSurface::update() {
	std::for_each(_cell.begin(), _cell.end(), [](Cell& c) {
		c.updateVelocity();
		c.updateHeight();
	});
	// std::for_each(_cell.begin(), _cell.end(), [](Cell& c) {
	// 	c.updateHeight();
	// });
}

void	WaterSurface::displayHeight() {
	for (int y = _sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < _sizeX; x++) {
			std::cout << std::fixed << std::setw(6) << std::setprecision(3) << _cell[index(x, y)].getHeight() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


void	WaterSurface::setHeight(int x, int y, float h) {
	if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY)
		return;
	_cell[index(x, y)].setHeight(h);
}