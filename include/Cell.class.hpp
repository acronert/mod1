#ifndef CELL_CLASS_HPP
# define CELL_CLASS_HPP

# include "mod1.hpp"
# include <iostream>

class Cell
{
	private:
		// Heights
		float	_h;
		float*	_hN;	// waterHeight N (pointer to neighbor cell N)
		float*	_hE;	// waterHeight E (pointer to neighbor cell E)
		float*	_hS;	// waterHeight S (pointer to neighbor cell S)
		float*	_hW;	// waterHeight W (pointer to neighbor cell W)

		// Velocities
		float	_vN;	// velocity N
		float	_vE;	// velocity W
		float*	_vS;	// velocity S (pointer to neighbor cell vN)
		float*	_vW;	// velocity E (pointer to neighbor cell vW)

		// Constants;

	public:
		Cell();
		Cell(float h, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell);
		~Cell();
		Cell(const Cell& other);
		Cell& operator=(const Cell& other);

		void	updateVelocity();
		void	updateHeight();

		float	getHeight();
		void	setHeight(float h);

};

#endif
