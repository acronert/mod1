#ifndef MAPGENERATOR_CLASS_HPP
# define MAPGENERATOR_CLASS_HPP

# include <vector>
# include <cmath>
# include <iostream>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <stb_image.h>

#define HEIGHT_COEFF 20.f	// image based map height coeff
#define INVERT 0			// invert image grey-scale

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

		void	generateMapFromImage(std::string& filepath);

		void	generateMapFromModFile(std::string& filepath);
		void	parseModInput(std::ifstream& file);
		float	findMapRatio();
		void	normalizePoints();
		float	IDWinterpolation(int x, int y, float power);

		void	displayPoints();
		std::vector<float>	getMap();
};

#endif
