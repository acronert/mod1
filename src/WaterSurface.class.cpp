#include "WaterSurface.class.hpp"


WaterSurface::WaterSurface(size_t sizeX, size_t sizeY, int cellSize)
	: _sizeX(sizeX), _sizeY(sizeY), _cellSize(cellSize)
{
	// _waveSpeed = std::min(WAVE_SPEED, static_cast<float>(0.5 * _cellSize / DELTA_TIME));
	_waveSpeed = 1;

	_k = (_waveSpeed * _waveSpeed) / (_cellSize * _cellSize);

	_heightMap = new float*[_sizeX];
	_previousHeightMap = new float*[_sizeX];
	_velocityMap = new float*[_sizeX];
	for (size_t x = 0; x < _sizeX; x++) {
		_heightMap[x] = new float[_sizeY](); // the () initialize to 0
		_previousHeightMap[x] = new float[_sizeY]();
		_velocityMap[x] = new float[_sizeY]();
	}
}

WaterSurface::~WaterSurface()
{
	for (size_t x = 0; x < _sizeX; x++) {
		delete[] _heightMap[x];
		delete[] _previousHeightMap[x];
		delete[] _velocityMap[x];
	}
	delete[] _heightMap;
	delete[] _previousHeightMap;
	delete[] _velocityMap;
}

WaterSurface::WaterSurface(const WaterSurface& other)
{
	_sizeX = other._sizeX;
	_sizeY = other._sizeY;
	_cellSize = other._cellSize;

	_heightMap = new float*[_sizeX];
	_previousHeightMap = new float*[_sizeX];
	_velocityMap = new float*[_sizeX];

	for (size_t x = 0; x < _sizeX; x++) {
		_heightMap[x] = new float[_sizeY]();
		_previousHeightMap[x] = new float[_sizeY]();
		_velocityMap[x] = new float[_sizeY]();
		for (size_t y = 0; y < _sizeY; y++) {
			_heightMap[x][y] = other._heightMap[x][y];
			_previousHeightMap[x][y] = other._previousHeightMap[x][y];
			_velocityMap[x][y] = other._velocityMap[x][y];
		}
	}
}

WaterSurface& WaterSurface::operator=(const WaterSurface& other)
{
	if (this != &other) {
		for (size_t x = 0; x < _sizeX; x++) {
			delete[] _heightMap[x];
			delete[] _previousHeightMap[x];
			delete[] _velocityMap[x];
		}
		delete[] _heightMap;
		delete[] _previousHeightMap;
		delete[] _velocityMap;

		_sizeX = other._sizeX;
		_sizeY = other._sizeY;
		_cellSize = other._cellSize;
		_k = other._k;

		_heightMap = new float*[_sizeX];
		_previousHeightMap = new float*[_sizeX];
		_velocityMap = new float*[_sizeX];

		for (size_t x = 0; x < _sizeX; x++) {
			_heightMap[x] = new float[_sizeY];
			_previousHeightMap[x] = new float[_sizeY];
			_velocityMap[x] = new float[_sizeY];
			for (size_t y = 0; y < _sizeY; y++) {
				_heightMap[x][y] = other._heightMap[x][y];
				_previousHeightMap[x][y] = other._previousHeightMap[x][y];
				_velocityMap[x][y] = other._velocityMap[x][y];
			}
		}
	}
	return *this;
}


void	WaterSurface::computeWaterMovement() {

	// copy the data
	for (size_t x = 0; x < _sizeX; x++) {
		for (size_t y = 0; y < _sizeY; y++) {
			_previousHeightMap[x][y] = _heightMap[x][y];
		}
	}

	// damping
	float posDamping;
	float velDamping; 
		// let pd = Math.min(this.posDamping * gPhysicsScene.dt, 1.0);
		// let vd = Math.max(0.0, 1.0 - this.velDamping * gPhysicsScene.dt);


	// For each cell
	for (size_t x = 0; x < this->_sizeX; x++) {
		for (size_t y = 0; y < this->_sizeY; y++) {

			// Calculate velocity
			// velocity[x, y] = velocity[x, y] + deltaTime * acceleration[x, y]
			_velocityMap[x][y] += DELTA_TIME * this->calculateCellAcceleration(x, y);


			// position damping
			_heightMap[x][y] += (0.25 * sum_height - height) * posDamping;
			// velocity damping
			_velocityMap[x][y] *= velDamping;

			// Calculate height
			// h[x, y] = h[x, y] + deltaTime * velocity[x, y]
			_heightMap[x][y] += DELTA_TIME * _velocityMap[x][y];
		}
	}
}

// Calculate acceleration
// acceleration[x, y] = k * (h[x-1, y] + h[x, y-1] + h[x+1, y], h[x, y+1] - 4 * h[x, y])
	// c is wave speed
	// s is cell size
	// k is (c^2 / s^2)
		// if a neighbor doesnt exist, set its height to h[x, y] instead
float	WaterSurface::calculateCellAcceleration(size_t x, size_t y) {
	int		sum_height = 0;
	int		height = this->_previousHeightMap[x][y]; 

	sum_height += x ? this->_previousHeightMap[x - 1][y] : height; 
	sum_height += x < this->_sizeX - 1 ? this->_previousHeightMap[x + 1][y] : height; 
	sum_height += y ? this->_previousHeightMap[x][y - 1] : height; 
	sum_height += y < this->_sizeY - 1 ? this->_previousHeightMap[x][y + 1] : height; 

	return this->_k * (sum_height - 4 * height);
}

void	WaterSurface::displayHeight() {
	for (size_t x = 0; x < this->_sizeX; x++) {
		for (size_t y = 0; y < this->_sizeY; y++) {
			std::cout << this->_heightMap[x][y] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void	WaterSurface::setCellHeight(size_t x, size_t y, int height) {
	if (x >= this->_sizeX || y >= this->_sizeY)
		return;
	_heightMap[x][y] = height;
}