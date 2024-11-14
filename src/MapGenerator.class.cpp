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

	generateMap();

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

			if (_heightMap[idx] == 0.0f) {
				_heightMap[idx] = IDWinterpolation(x, y, 4.0f);
				// _heightMap[idx] = RBFinterpolation(x, y, 0.0025f);
			}
		}
	}

}

float	MapGenerator::RBFinterpolation(int x, int y, float epsilon) {
	float	result = 0.0f;
	float	denominator = 0.0f;

	// for each known points
	for (std::vector<s_coord>::iterator it = _points.begin(); it != _points.end(); it++) {
		// distance between the point and the target
		float	distance = std::sqrt(std::pow(x - it->x, 2) + std::pow(y - it->y, 2));
		// calculate weight using gaussian rbf
		float	weight = gaussianKernel(distance, epsilon);
		// sum up weight the value and sum up
		result += it->z * weight;
		// sum up the weight to get the denominator
		denominator += weight;
	}

	return result / denominator;
}

float	MapGenerator::gaussianKernel(float distance, float epsilon) {
	return std::exp(-epsilon * distance * distance);
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

		while (iss >> c1) {
			// check opening parenthesis
			if ( c1 != '(')
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


float MapGenerator::IDWinterpolation(int x, int y, float power) {
    float result = 0.0f;
    float weightSum = 0.0f;
    
    // Guard against empty points
    if (_points.empty()) {
        return 0.0f;
    }

    for (const auto& point : _points) {
        float dx = x - point.x;
        float dy = y - point.y;
        float distSquared = dx*dx + dy*dy;
        
        // If we're exactly on a known point, return its value
        if (distSquared < 1e-6f) {
            return point.z;
        }
        
        // Calculate weight as inverse of distance squared
        float weight = 1.0f / std::pow(distSquared, power/2.0f);
        
        result += point.z * weight;
        weightSum += weight;
    }
    
    if (weightSum < 1e-6f) {
        return 0.0f;
    }
    
    return result / weightSum;
}



// float MapGenerator::bilinearInterpolation(int x, int y, 
//                                         const std::vector<std::vector<GridPoint>>& grid,
//                                         int gridSize) {
//     // Convert to grid coordinates
//     float gx = static_cast<float>(x) / gridSize;
//     float gy = static_cast<float>(y) / gridSize;
    
//     // Get grid cell indices
//     int x1 = static_cast<int>(std::floor(gx));
//     int y1 = static_cast<int>(std::floor(gy));
//     int x2 = x1 + 1;
//     int y2 = y1 + 1;
    
//     // Calculate interpolation weights
//     float wx = gx - x1;
//     float wy = gy - y1;
    
//     // Ensure we have valid points
//     if (x1 >= 0 && x2 < grid.size() && y1 >= 0 && y2 < grid[0].size()) {
//         // Get corner values
//         float v11 = grid[x1][y1].valid ? grid[x1][y1].value : 0.0f;
//         float v21 = grid[x2][y1].valid ? grid[x2][y1].value : 0.0f;
//         float v12 = grid[x1][y2].valid ? grid[x1][y2].value : 0.0f;
//         float v22 = grid[x2][y2].valid ? grid[x2][y2].value : 0.0f;
        
//         // Bilinear interpolation
//         float result = v11 * (1-wx) * (1-wy) +
//                       v21 * wx * (1-wy) +
//                       v12 * (1-wx) * wy +
//                       v22 * wx * wy;
                      
//         return result;
//     }
    
//     return 0.0f;
// }