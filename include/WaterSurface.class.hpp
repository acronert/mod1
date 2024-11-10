#ifndef WATERSURFACE_CLASS_HPP
# define WATERSURFACE_CLASS_HPP

#include "Cell.struct.hpp"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unistd.h> 

#define DELTA_TIME 1.0f
#define WAVE_SPEED 0.5f
#define POSITION_DAMPING 0.8
#define VELOCITY_DAMPING 0.8

class WaterSurface
{
	private:
		int		_sizeX;
		int		_sizeY;
		float	_cellSize;	// in meters
		float	_waveSpeed;	// in meters/seconds
		float	_k;			// propagation constant
		float	_dt;		// delta time
		float	_halfLife;

		std::vector<Cell> _cell;

	public:
		WaterSurface(int sizeX, int sizeY);
		~WaterSurface();
		WaterSurface(const WaterSurface& other);
		WaterSurface& operator=(const WaterSurface& other);

		int		index(int x, int y);
		void	setCellHeight(int x, int y, int h);

		float	acceleration(Cell& target, int neighborX, int neighborY);

		void	update();

		void	displayHeight();
		float	getSumHeight();

		std::vector<Cell>	getCells();
};

#endif
