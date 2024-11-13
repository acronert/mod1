#ifndef MOD1_H
# define MOD1_H

# include <cmath>



// _waveSpeed = std::min(WAVE_SPEED, static_cast<float>(0.5 * _cellSize / DELTA_TIME));

# define CELL_SIZE		1.0f			//	in meter
# define DELTA_TIME		1.0f / 60.0f		//	in sec
# define HALF_LIFE		40.0f			//	in sec

# define WAVE_SPEED		2.0f				//	in meter/sec
// # define WAVE_SPEED		std::min(2.0f, 0.5f * CELL_SIZE / DELTA_TIME)	//	in meter/sec

# define PROPAGATION	(WAVE_SPEED * WAVE_SPEED) / (CELL_SIZE * CELL_SIZE)
# define DAMPENING		pow(0.5f, DELTA_TIME / HALF_LIFE) // near 0 for more dampening



#endif