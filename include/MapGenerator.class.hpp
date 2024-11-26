#ifndef MAPGENERATOR_CLASS_HPP
# define MAPGENERATOR_CLASS_HPP

# include <vector>
# include <cmath>
# include <iostream>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <stb_image.h>

#define HEIGHT_COEFF 20.f
#define INVERT 1

struct s_coord {
	int	x;
	int	y;
	int	z;
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

		void	generateMapFromModFile(std::string& filepath);
		void	parseModInput(std::ifstream& file);
		float	IDWinterpolation(int x, int y, float power);

		void	generateMapFromImage(std::string& filepath);

		std::vector<float>	getMap();
		void	displayPoints();
};

#endif
