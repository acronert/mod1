#ifndef MOD1_H
# define MOD1_H

# include <cmath>

// _waveSpeed = std::min(WAVE_SPEED, static_cast<float>(0.5 * _cellSize / DELTA_TIME));
# define WAVE_SPEED		2.0f				//	in meter/sec
# define CELL_SIZE		0.001f				//	in meter
# define DELTA_TIME		1.0f / 10000.0f		//	in sec
# define HALF_LIFE		0.07f
# define PROPAGATION	(WAVE_SPEED * WAVE_SPEED) / (CELL_SIZE * CELL_SIZE)
# define DAMPENING		pow(0.3f, DELTA_TIME / HALF_LIFE)

#endif