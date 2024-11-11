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
			_cell[index(x, y)] = Cell(1.0f, 0.0f, Ncell, Ecell, Scell, Wcell);
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
		c.updateWaterLevel();
	});

	// check underflow
	for (int x = 0; x < _sizeX; x++) {
		for (int y = 0; y < _sizeX; y++) {
			if (_cell[index(x, y)].getWaterLevel() < 0)
				checkUnderflow(x, y);
		}
	}
}

void	WaterSurface::checkUnderflow(int x, int y) {
	if (x < 0 || y < 0 || x >= _sizeX || y >= _sizeY || _cell[index(x, y)].getWaterLevel() >= 0)
		return;

	std::cout << "Underflow in [" << x << "][" << y << "]" << std::endl;

	_cell[index(x,y)].resolveUnderflow();

	// recursive call
	checkUnderflow(x-1, y);
	checkUnderflow(x+1, y);
	checkUnderflow(x, y-1);
	checkUnderflow(x, y+1);
}

void	WaterSurface::displayWaterLevel() {
	std::cout << "===== Water Level =====" << std::endl;
	for (int y = _sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < _sizeX; x++) {
			std::cout << std::fixed << std::setw(6) << std::setprecision(3) << _cell[index(x, y)].getWaterLevel() << " ";
			std::cout << std::defaultfloat;
		}
		std::cout << std::endl;
	}
	std::cout << "TotalWaterLevel = " << getTotalWaterLevel() << std::endl;
	std::cout << std::endl;
}

void	WaterSurface::displayGroundLevel() {
	std::cout << "===== Ground Level =====" << std::endl;
	for (int y = _sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < _sizeX; x++) {
			std::cout << std::fixed << std::setw(6) << std::setprecision(3) << _cell[index(x, y)].getGroundLevel() << " ";
			std::cout << std::defaultfloat;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void	WaterSurface::displayASCII() {
	float	min = 0.0f;
	float	max = 2.0f;
	int		color;
	float	delta = (max - min) / 24.0f;

	for (int y = _sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < _sizeX; x++) {
			color = floor(_cell[index(x, y)].getWaterLevel() / delta);
			if (color < 0)
				color = -36;
			else if (color > 23)
				color = 23;
			std::cout << "\033[48;5;" << color + 232 << "m" << "  " << "\033[0m";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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


void	WaterSurface::displayCellInfo(int x, int y) {
	Cell& cell = _cell[index(x, y)];

	std::cout << "Cell[" << x << "][" << y << "] Info = " << std::endl;
	std::cout << "      w = " << cell.getWaterLevel() << std::endl;
	std::cout << "      g = " << cell.getGroundLevel() << std::endl;
	std::cout << "      vN = " << cell.getVelocityN() << std::endl;
	std::cout << "      vE = " << cell.getVelocityE() << std::endl;
	std::cout << "      vS = " << cell.getVelocityS() << std::endl;
	std::cout << "      vW = " << cell.getVelocityW() << std::endl;

}