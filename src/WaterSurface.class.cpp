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




	// For each cell
	for (size_t x = 0; x < this->_sizeX; x++) {
		for (size_t y = 0; y < this->_sizeY; y++) {


			// Calculate velocity
			// velocity[x, y] = velocity[x, y] + deltaTime * acceleration[x, y]
			// _velocityMap[x][y] += DELTA_TIME * this->calculateCellAcceleration(x, y);
			int		sum_height = 0;
			// int		height = this->_previousHeightMap[x][y];
			int		height = std::max(0.0f, this->_previousHeightMap[x][y]); 

			sum_height += x ? std::max(0.0f, this->_previousHeightMap[x - 1][y]) : height; 
			sum_height += x < this->_sizeX - 1 ? std::max(0.0f, this->_previousHeightMap[x + 1][y]) : height; 
			sum_height += y ? std::max(0.0f, this->_previousHeightMap[x][y - 1]) : height; 
			sum_height += y < this->_sizeY - 1 ? std::max(0.0f, this->_previousHeightMap[x][y + 1]) : height; 

			_velocityMap[x][y] += DELTA_TIME * this->_k * (sum_height - 4 * height);


			// damping
			float posDamping = std::min(POSITION_DAMPING * DELTA_TIME, 1.0);
			float velDamping = std::max(0.0, 1.0 - VELOCITY_DAMPING * DELTA_TIME);
			// position damping
			_heightMap[x][y] += (0.25 * sum_height - height) * posDamping;
			// velocity damping
			_velocityMap[x][y] *= velDamping;

			// Calculate height
			// h[x, y] = h[x, y] + deltaTime * velocity[x, y]
			_heightMap[x][y] += DELTA_TIME * _velocityMap[x][y];


			// borrow  water if height is negative
			// if (_heightMap[x][y] < 0)
				// borrowWater(x, y);
		}
	}
}

void WaterSurface::borrowWater(size_t x, size_t y) {
    float heightDebt = -_heightMap[x][y];
    float borrow = 0;

    if (x) {
        float amountToBorrow = std::min(std::max(0.0f, _heightMap[x - 1][y]), heightDebt - borrow);
        _heightMap[x - 1][y] -= amountToBorrow;
        borrow += amountToBorrow;
    }
    if (x < _sizeX - 1 && borrow < heightDebt) {
        float amountToBorrow = std::min(std::max(0.0f, _heightMap[x + 1][y]), heightDebt - borrow);
        _heightMap[x + 1][y] -= amountToBorrow;
        borrow += amountToBorrow;
    }
    if (y && borrow < heightDebt) {
        float amountToBorrow = std::min(std::max(0.0f, _heightMap[x][y - 1]), heightDebt - borrow);
        _heightMap[x][y - 1] -= amountToBorrow;
        borrow += amountToBorrow;
    }
    if (y < _sizeY - 1 && borrow < heightDebt) {
        float amountToBorrow = std::min(std::max(0.0f, _heightMap[x][y + 1]), heightDebt - borrow);
        _heightMap[x][y + 1] -= amountToBorrow;
        borrow += amountToBorrow;
    }

    // Update the height of the current cell
    _heightMap[x][y] += borrow;
}


// void	WaterSurface::computeWaterMovement() {

// 	// copy the data
// 	for (size_t x = 0; x < _sizeX; x++) {
// 		for (size_t y = 0; y < _sizeY; y++) {
// 			_previousHeightMap[x][y] = _heightMap[x][y];
// 		}
// 	}

// 	// damping
// 	float posDamping = std::min(POSITION_DAMPING * DELTA_TIME, 1.0);
// 	float velDamping = std::max(0.0, 1.0 - VELOCITY_DAMPING * DELTA_TIME);


// 	// For each cell
// 	for (size_t x = 0; x < this->_sizeX; x++) {
// 		for (size_t y = 0; y < this->_sizeY; y++) {


// 			// Calculate velocity
// 			// velocity[x, y] = velocity[x, y] + deltaTime * acceleration[x, y]
// 			// _velocityMap[x][y] += DELTA_TIME * this->calculateCellAcceleration(x, y);
// 			int		sum_height = 0;
// 			int		height = std::max(0.0f, this->_previousHeightMap[x][y]); 

// 			// sum_height += x ? this->_previousHeightMap[x - 1][y] : height; 
// 			// sum_height += x < this->_sizeX - 1 ? this->_previousHeightMap[x + 1][y] : height; 
// 			// sum_height += y ? this->_previousHeightMap[x][y - 1] : height; 
// 			// sum_height += y < this->_sizeY - 1 ? this->_previousHeightMap[x][y + 1] : height; 
// 			sum_height += x ? std::max(0.0f, this->_previousHeightMap[x - 1][y]) : height; 
// 			sum_height += x < this->_sizeX - 1 ? std::max(0.0f, this->_previousHeightMap[x + 1][y]) : height; 
// 			sum_height += y ? std::max(0.0f, this->_previousHeightMap[x][y - 1]) : height; 
// 			sum_height += y < this->_sizeY - 1 ? std::max(0.0f, this->_previousHeightMap[x][y + 1]) : height; 

// 			_velocityMap[x][y] += DELTA_TIME * this->_k * (sum_height - 4 * height);


			
// 			// position damping
// 			_heightMap[x][y] += (0.25 * sum_height - height) * posDamping;
// 			// velocity damping
// 			_velocityMap[x][y] *= velDamping;

// 			// Calculate height
// 			// h[x, y] = h[x, y] + deltaTime * velocity[x, y]
// 			_heightMap[x][y] += DELTA_TIME * _velocityMap[x][y];
// 			// test to avoid neg values
// 				if (_heightMap[x][y] < 0)
// 					_heightMap[x][y] = 0;
// 		}
// 	}
// }

// Calculate acceleration
// acceleration[x, y] = k * (h[x-1, y] + h[x, y-1] + h[x+1, y], h[x, y+1] - 4 * h[x, y])
	// c is wave speed
	// s is cell size
	// k is (c^2 / s^2)
		// if a neighbor doesnt exist, set its height to h[x, y] instead
// float	WaterSurface::calculateCellAcceleration(size_t x, size_t y) {
// 	int		sum_height = 0;
// 	int		height = this->_previousHeightMap[x][y]; 

// 	sum_height += x ? this->_previousHeightMap[x - 1][y] : height; 
// 	sum_height += x < this->_sizeX - 1 ? this->_previousHeightMap[x + 1][y] : height; 
// 	sum_height += y ? this->_previousHeightMap[x][y - 1] : height; 
// 	sum_height += y < this->_sizeY - 1 ? this->_previousHeightMap[x][y + 1] : height; 

// 	return this->_k * (sum_height - 4 * height);
// }

void WaterSurface::displayHeight() {
    // Define the minimum and maximum values for the gradient range
    float minValue = -1.0f;  // Adjust to the lowest expected height
    float maxValue = 8.0f;   // Adjust to the highest expected height

    for (size_t x = 0; x < this->_sizeX; x++) {
        for (size_t y = 0; y < this->_sizeY; y++) {
            float value = this->_heightMap[x][y];

            // Normalize value to a range between 0 and 1
            float normalized = (value - minValue) / (maxValue - minValue);
            if (normalized < 0) normalized = 0;
            if (normalized > 1) normalized = 1;

            // Map normalized value to a color gradient
            int colorCode;
            if (normalized < 0.14) {
                colorCode = 31;  // Dark Red
            } else if (normalized < 0.28) {
                colorCode = 91;  // Red
            } else if (normalized < 0.42) {
                colorCode = 33;  // Yellow
            } else if (normalized < 0.57) {
                colorCode = 32;  // Green
            } else if (normalized < 0.71) {
                colorCode = 36;  // Cyan
            } else if (normalized < 0.85) {
                colorCode = 34;  // Blue
            } else {
                colorCode = 35;  // Magenta
            }

            // Set color and print the value
            // std::cout << "\033[" << colorCode << "m" << std::fixed << value << " ";
            std::cout << "\033[" << colorCode << "m" << std::fixed << "O" << " ";
        }
        std::cout << "\033[0m" << std::endl; // Reset color after each row
    }
    std::cout << "\033[0m" << std::endl; // Reset color at the end
}

// void	WaterSurface::displayHeight() {
// 	for (size_t x = 0; x < this->_sizeX; x++) {
// 		for (size_t y = 0; y < this->_sizeY; y++) {
// 			std::cout << this->_heightMap[x][y] << " ";
// 		}
// 		std::cout << std::endl;
// 	}
// 	std::cout << std::endl;
// }

void	WaterSurface::setCellHeight(size_t x, size_t y, int height) {
	if (x >= this->_sizeX || y >= this->_sizeY)
		return;
	_heightMap[x][y] = height;
}

float WaterSurface::getHeightSum() {
	float sum = 0;
	for (size_t x = 0; x < this->_sizeX; x++) {
		for (size_t y = 0; y < this->_sizeY; y++) {
			sum += _heightMap[x][y];
		}
	}
	return sum;
}