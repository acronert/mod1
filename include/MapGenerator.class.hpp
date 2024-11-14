#ifndef MAPGENERATOR_CLASS_HPP
# define MAPGENERATOR_CLASS_HPP

# include <vector>
# include <cmath>
# include <iostream>
# include <fstream>
# include <sstream>
# include <iomanip>

struct s_coord {
	int	x;
	int	y;
	int	z;
};

struct GridPoint {
    bool valid = false;
    float value = 0.0f;
};


class MapGenerator
{
	private:
		std::vector<float>		_heightMap;
		int						_size;

		std::vector<s_coord>	_points;

	public:
		MapGenerator();
		MapGenerator(std::string filepath, int rendererSize);
		~MapGenerator();
		// MapGenerator(const MapGenerator& other);
		// MapGenerator& operator=(const MapGenerator& other);

		float	findMapRatio();
		void	normalizePoints();

		void	generateMap();
		float	RBFinterpolation(int x, int y, float epsilon);
		float	gaussianKernel(float distance, float epsilon);

		void	parseInput(std::string& filepath);
		std::vector<float>	getMap();


// float bilinearInterpolation(int x, int y, 
//                                         const std::vector<std::vector<GridPoint>>& grid,
//                                         int gridSize);
float IDWinterpolation(int x, int y, float power);

		void	displayPoints();
};

#endif
