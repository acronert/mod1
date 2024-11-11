#include "WaterSurface.class.hpp"

#define WIDTH 10
#define HEIGHT 10
#define ITER 10

int	main() {

	WaterSurface surface(WIDTH, HEIGHT);

	surface.setWaterLevel(2, 2, 10.0f);
	surface.displayWaterLevel();

	// Iteration
	for (int i = 0; i < ITER; i++) {
		surface.update();
		surface.displayWaterLevel();
	}

	// Infinite iteration
	// while (1) {
	// 	surface.update();
	// 	// surface.displayWaterLevel();
	// 	surface.displayASCII();
	// 	usleep(250000); // 0.25sec
	// 	system("clear");
	// }

}
