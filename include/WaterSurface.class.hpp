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

class WaterSurface
{
	private:
		int		_sizeX;
		int		_sizeY;
		// float	_cellSize;	// in meters
		// float	_waveSpeed;	// in meters/seconds
		// float	_k;			// propagation constant
		// float	_dt;		// delta time
		// float	_halfLife;

		std::vector<Cell> _cell;

	public:
		WaterSurface(int sizeX, int sizeY);
		~WaterSurface();
		WaterSurface(const WaterSurface& other);
		WaterSurface& operator=(const WaterSurface& other);

		void	checkUnderflow(int x, int y);

		int		index(int x, int y);
		void	displayWaterLevel();
		void	displayGroundLevel();
		void	displayASCII();
		void	displayCellInfo(int x, int y);

		void	setGroundLevel(int x, int y, float h);
		void	setWaterLevel(int x, int y, float h);
		float	getTotalWaterLevel();

		void	update();


};

#endif
