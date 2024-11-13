#ifndef INPUT_STRUCT_HPP
# define INPUT_STRUCT_HPP

struct s_input {
	// keyboard input
	bool	forward = false;
	bool	backward = false;
	bool	left = false;
	bool	right = false;
	bool	up = false;
	bool	down = false;
	bool	yawLeft = false;
	bool	yawRight = false;
	bool	pitchUp = false;
	bool	pitchDown = false;
};

#endif