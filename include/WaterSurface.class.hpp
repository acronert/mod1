#ifndef WATERSURFACE_CLASS_HPP
# define WATERSURFACE_CLASS_HPP

#include "Cell.class.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unistd.h> 
#include <cstdlib> // rand 

class WaterSurface
{
	private:
		int		_sizeX;
		int		_sizeY;

		std::vector<Cell> _cell;

	public:
		WaterSurface(int sizeX, int sizeY);
		~WaterSurface();
		WaterSurface(const WaterSurface& other);
		WaterSurface& operator=(const WaterSurface& other);

		void	checkUnderflow(int x, int y);

		int		index(int x, int y);
		void	displayASCII();

		void	setGroundLevel(int x, int y, float h);
		void	setWaterLevel(int x, int y, float h);

		std::vector<Cell>&	getCells();
		float	getTotalWaterLevel();
		int		getSizeX();
		int		getSizeY();

		float	getWaterVertexHeight(int x, int y);

		void	update();
		void	updateGroundNormal();

		void	loadGroundMap(const std::vector<float>& heightmap);

		void	resetWater();
		void	riseWater(float intensity, float threshold);
		void	makeRain(float rainIntensity, float dropletSize);
		void	makeWave(float intensity);
		void	flush(bool N, bool S, bool E, bool W);
};

#endif
