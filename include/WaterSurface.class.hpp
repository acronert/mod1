#ifndef WATERSURFACE_CLASS_HPP
# define WATERSURFACE_CLASS_HPP

#include "Cell.struct.hpp"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h> 

#define DELTA_TIME 1.0f
#define WAVE_SPEED 0.5f
#define POSITION_DAMPING 0.8
#define VELOCITY_DAMPING 0.8

class WaterSurface
{
	private:
		size_t	_sizeX;
		size_t	_sizeY;
		float	_cellSize;
		float	_waveSpeed;
		float	_k;

		// Cell**	_grid;
		float** _heightMap;
		float** _previousHeightMap;
		float** _velocityMap;

	public:
		WaterSurface(size_t sizeX, size_t sizeY, int spacing);
		~WaterSurface();
		WaterSurface(const WaterSurface& other);
		WaterSurface& operator=(const WaterSurface& other);

		void	computeWaterMovement();
		// float	calculateCellAcceleration(size_t x, size_t y);
		void	setCellHeight(size_t x, size_t y, int height);
		void	borrowWater(size_t x, size_t y);

		void	displayHeight();
		float	getHeightSum();

};

#endif
