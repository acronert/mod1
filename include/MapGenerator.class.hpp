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
		std::vector<s_coord>	_points;
		int						_size;

	public:
		MapGenerator();
		MapGenerator(std::string filepath, int rendererSize);
		~MapGenerator();

		float	findMapRatio();
		void	normalizePoints();

		void	generateMap();
		float	IDWinterpolation(int x, int y, float power);

		void	parseInput(std::string& filepath);
		std::vector<float>	getMap();

		void	displayPoints();
};

#endif
