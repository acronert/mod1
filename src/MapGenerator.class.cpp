#include "MapGenerator.class.hpp"

MapGenerator::MapGenerator()
{

}

MapGenerator::MapGenerator(std::string filepath, int rendererSize) : _size(rendererSize)
{
	parseInput(filepath);
	std::cout << "Real points:" << std::endl;
	displayPoints();

	normalizePoints();
	std::cout << "Resized points:" << std::endl;
	displayPoints();

	// Create map using bicubic interpolation
	generateMap();

	int x =0;
	for (std::vector<float>::iterator it = _heightMap.begin(); it != _heightMap.end(); it++) {
		std::cout << std::fixed << std::setprecision(2) << std::setw(8) << *it << " ";
		if (x < _size -1)
			x++;

		else {
			std::cout << std::endl;
			x = 0;
		}
	}

}

MapGenerator::~MapGenerator()
{

}

// MapGenerator::MapGenerator(const MapGenerator& other)
// {
// 	// _value = other.value;
// 	// _ptr = new int;
// 	// *_ptr = *source._ptr;
// }

// MapGenerator& MapGenerator::operator=(const MapGenerator& other)
// {
// 	if (this != &other)
// 	{
// 		// _value = other.value;

// 		// delete  _ptr;
// 		// _ptr = new int;
// 		// *_ptr = *source._ptr;
// 	}
// 	return (*this);
// }



void	MapGenerator::generateMap() {
	_heightMap.resize(_size * _size, 0.0f);

	// set known points
	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		int	idx = it->x + it->y * _size;
		_heightMap[idx] = static_cast<float>(it->z);
	}

	// interpolate the missing heights
	for (int y = 0; y  < _size; y++) {
		for (int x = 0; x  < _size; x++) {
			int idx = x + y * _size;

			if (_heightMap[idx] == 0.0f) {
				_heightMap[idx] = bicubicInterpolate(x, y);
			}
		}
	}
}

float	MapGenerator::bicubicInterpolate(int x, int y) {
	float x_t = static_cast<float>(x) / (_size - 1);
	float y_t = static_cast<float>(y) / (_size - 1);

	std::vector<std::vector<float>> heights(4, std::vector<float>(4, 0.0f));

	// Get the 16 surrounding heights
	for (int dy = -1; dy <= 2; ++dy) {
		for (int dx = -1; dx <= 2; ++dx) {
			int xx = x + dx;
			int yy = y + dy;
			xx = std::max(0, std::min(xx, _size - 1));
			yy = std::max(0, std::min(yy, _size - 1));
			int index = xx + yy * _size;
			heights[dx + 1][dy + 1] = _heightMap[index];
		}
	}

	float height = 0.0f;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			height += heights[i][j] * bicubicKernel(i - 1, x_t) * bicubicKernel(j - 1, y_t);
		}
	}

	return height;
}

float	MapGenerator::bicubicKernel(int x, float t) {
	float a = 0.5f;
	if (x == -1)
		return (a + 2) * t * t * t - (a + 3) * t * t + 1;
	else if (x == 0)
		return a * t * t * t - 5 * a * t * t + 8 * a * t - 4 * a;
	else if (x == 1)
		return a * t * t * t - 4 * a * t * t + 7 * a * t - 2 * a;
	else if (x == 2)
		return (a - 2) * t * t * t + 3 * (a - 2) * t * t - 6 * (a - 2) * t + 4 * (a - 2);
	else
		return 0.0f;
}


// The implied size is the size of the map that is implied by the .mod1 file
float	MapGenerator::findMapRatio() {
	int	largestX = 0;
	int	largestY = 0;
	int	smallestX = -1;
	int	smallestY = -1;
	int	impliedSize;

	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		if (smallestX == -1)
			smallestX = it->x;
		if (smallestY == -1)
			smallestY = it->y;

		if (it->x > largestX)
			largestX = it->x;
		else if (it->x < smallestX)
			smallestX = it->x;

		if (it->y > largestY)
			largestY = it->y;
		else if (it->y < smallestY)
			smallestY = it->y;
	}

	impliedSize = std::max(smallestX + largestX, smallestY + largestY);

	return static_cast<float>(_size) / impliedSize;
}

void	MapGenerator::normalizePoints() {

	float ratio = findMapRatio();
	std::cout << "ratio = " << ratio << std::endl;

	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		it->x *= ratio;
		it->y *= ratio;
		it->z *= ratio;
	}
}

void	MapGenerator::parseInput(std::string& filepath) {
	if (_size < 0)
		throw std::invalid_argument("Invalid arguments: size must me positive");

	size_t	pos = filepath.find_last_of(".");
	if (pos == std::string::npos || filepath.substr(pos) != ".mod1")
		throw std::invalid_argument("Invalid filepath extension");

	std::ifstream file(filepath);

	if (!file)
		throw std::invalid_argument("Invalid filepath");

	std::string	line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		char c1, c2;
		s_coord coord;

		// check opening parenthesis
		if (!(iss >> c1) || c1 != '(')
			throw std::invalid_argument("Invalid file content: expected '('");
		// Read coordinates
		if (!(iss >> coord.x >> c1 >> coord.y >> c2 >> coord.z) || c1 != ',' || c2 != ',')
			throw std::invalid_argument("Invalid file content: expected 'x,y,z'");
		// check closing parenthesis
		if (!(iss >> c1) || c1 != ')')
			throw std::invalid_argument("Invalid file content: expected ')'");

		if (coord.x < 0 || coord.y < 0 || coord.z < 0)
			throw std::invalid_argument("Invalid file content: x,y and z must be positive");
		_points.push_back(coord);
	}

	// CHECK SI DEUX POINTS ONT LES MEMES X ET Y ??

	file.close();
}

std::vector<float>	MapGenerator::getMap() { return _heightMap; };

void	MapGenerator::displayPoints() {
	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		std::cout << "x = " << it->x << ", y = " << it->y << ", z = " << it->z << std::endl;
	}
	
}