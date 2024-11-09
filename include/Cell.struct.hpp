#ifndef CELL_STRUCT_HPP
# define CELL_STRUCT_HPP

struct Cell {
	int		height;
	int		next_height;
	float	velocity;

	Cell(int h = 0, float v = 0) : height(h), velocity(v) {}
};

#endif
