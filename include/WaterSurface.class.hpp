#ifndef WATERSURFACE_CLASS_HPP
# define WATERSURFACE_CLASS_HPP

#include "Cell.struct.hpp"
#include <stdio.h>
#include <iostream>
#include <algorithm>

#define DELTA_TIME 1.0f
#define WAVE_SPEED 10.0f

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
		float	calculateCellAcceleration(size_t x, size_t y);
		void	setCellHeight(size_t x, size_t y, int height);

		void	displayHeight();


};

#endif
