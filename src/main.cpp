#include "WaterSurface.class.hpp"

#define WIDTH 50
#define HEIGHT 50
#define ITER 10

int	main() {

	WaterSurface surface(WIDTH, HEIGHT);



	for (int i = WIDTH / 5 ; i < 4 * WIDTH /5; i++) {
		surface.setGroundLevel(HEIGHT / 2, i, 1.5f);
		surface.setWaterLevel(HEIGHT / 2, i, 0.0f);
		surface.setWaterLevel(0, i, 15.0f);
	}
	surface.setWaterLevel(30, 30, 8.0f);

	// surface.displayWaterLevel();
	// surface.displayGroundLevel();

	// Iteration
	// for (int i = 0; i < ITER; i++) {
	// 	surface.update();
	// 	surface.displayWaterLevel();
	// 	// surface.displayASCII();
	// 	// surface.displayCellVelocities(25,25);
	// 	// surface.displayGroundLevel();
	// }

	// // Infinite iteration
	while (1) {
		surface.update();
		// surface.displayWaterLevel();
		surface.displayASCII();
		std::cout << std::fixed <<"TotalWaterLevel = " << surface.getTotalWaterLevel() << std::endl;
		usleep(250000);
		// system("clear");
	}

}
