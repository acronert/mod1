#include "MapGenerator.class.hpp"

MapGenerator::MapGenerator() : _size(100) {}

MapGenerator::MapGenerator(std::string filepath, int rendererSize) : _size(rendererSize)
{
	parseInput(filepath);
	std::cout << "Real points:" << std::endl;
	displayPoints();

	normalizePoints();
	std::cout << "Resized points:" << std::endl;
	displayPoints();

	generateMap();
}

MapGenerator::~MapGenerator() {}

void	MapGenerator::generateMap() {
	_heightMap.resize(_size * _size, 0.0f);

	//add border points to known points ( don't duplicates the corners)
	for (int x = 0; x < _size; x++) {
		for (int y = 0; y < _size; y++) {
			if (!x || !y || x == _size - 1 || y == _size - 1)
				_points.push_back({x, y, 0});
		}
	}
	// interpolate the missing heights
	for (int y = 0; y  < _size; y++) {
		for (int x = 0; x  < _size; x++) {
			int idx = x + y * _size;
			
			if (_heightMap[idx] == 0.0f)
				_heightMap[idx] = IDWinterpolation(x, y, 4.0f);
		}
	}
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

		while (iss >> c1) {
			if ( c1 != '(')
				throw std::invalid_argument("Invalid file content: expected '('");
			if (!(iss >> coord.x >> c1 >> coord.y >> c2 >> coord.z) || c1 != ',' || c2 != ',')
				throw std::invalid_argument("Invalid file content: expected 'x,y,z'");
			if (!(iss >> c1) || c1 != ')')
				throw std::invalid_argument("Invalid file content: expected ')'");
			if (coord.x < 0 || coord.y < 0 || coord.z < 0)
				throw std::invalid_argument("Invalid file content: x,y and z must be positive");
			_points.push_back(coord);
		}
	}
	file.close();
}

std::vector<float>	MapGenerator::getMap() { return _heightMap; };

void	MapGenerator::displayPoints() {
	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		std::cout << "x = " << it->x << ", y = " << it->y << ", z = " << it->z << std::endl;
	}
}

// inverse distance weighting interpolation
float MapGenerator::IDWinterpolation(int x, int y, float power) {
	float result = 0.0f;
	float weightSum = 0.0f;
	
	if (_points.empty()) {
		return 0;
	}
	for (const auto& point : _points) {
		float dx = x - point.x;
		float dy = y - point.y;
		float distSquared = dx*dx + dy*dy;
		
		if (distSquared == 0) {
			return point.z;
		}
		
		float weight = 1.0f / std::pow(distSquared, power/2.0f);
		
		result += point.z * weight;
		weightSum += weight;
	}
	
	if (weightSum == 0) {
		return 0.0f;
	}
	
	return result / weightSum;
}
