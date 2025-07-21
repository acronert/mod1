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
	bool	plus = false;
	bool	minus = false;

	// simulation controls
	bool	pause = false;
	bool	rain_mode = false;
	bool	rise_mode = false;
	bool	wave_mode = false;
	bool	flush_mode = false;

	int		scroll = 0;

	bool	reset_water = false;
};

#endif