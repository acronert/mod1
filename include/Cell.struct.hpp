#ifndef CELL_STRUCT_HPP
# define CELL_STRUCT_HPP

struct Cell {
	float	h;
	float	v;
	float	a;

	Cell(int height = 0, float velocity = 0, float acceleration = 0)
		: h(height), v(velocity), a(acceleration) {}
};

#endif
