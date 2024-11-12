#include "WaterSurface.class.hpp"
#include "Renderer.class.hpp"

#include <ctime>

#define WIDTH 200
#define HEIGHT 200
#define ITER 100





int	main() {

	Renderer renderer;

	WaterSurface surface(WIDTH, HEIGHT);

	// add water
	surface.setWaterLevel(WIDTH / 2, HEIGHT / 2, 100);

	// add ground
	for (int i = HEIGHT / 5; i < 4 * HEIGHT / 5; i++) {
		surface.setGroundLevel(i /2 + WIDTH/3 -2, i, 3.0f);
		surface.setGroundLevel(i /2 + WIDTH/3 -1, i, 3.5f);
		surface.setGroundLevel(i /2 + WIDTH/3, i, 4.0f);
		surface.setGroundLevel(i /2 + WIDTH/3 + 1, i, 3.5f);
		surface.setGroundLevel(i /2 + WIDTH/3 + 2, i, 3.0f);
	}

	for (int i = 0; i < WIDTH / 5; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			surface.setGroundLevel(i, j, (WIDTH / 5) - i);
		}
	}



	renderer.render(surface);

	
}


// int	main() {

// 	Renderer	renderer;
// 	renderer.initWindow(600, 400, "mod1");

// 	WaterSurface surface(WIDTH, HEIGHT);

// 	for (int i = WIDTH / 5 ; i < 4 * WIDTH /5; i++) {
// 		surface.setGroundLevel(HEIGHT / 2, i, 1.5f);
// 		surface.setWaterLevel(HEIGHT / 2, i, 0.0f);
// 		surface.setWaterLevel(0, i, 15.0f);
// 	}
// 	surface.setWaterLevel(60, 30, 40.0f);

// 	// surface.displayWaterLevel();
// 	// surface.displayGroundLevel();

// 	// Iteration
// 	// for (int i = 0; i < ITER; i++) {
// 	// 	surface.update();
// 	// 	// surface.displayWaterLevel();
// 	// 	surface.displayASCII();
// 	// 	// surface.displayCellVelocities(25,25);
// 	// 	// surface.displayGroundLevel();
// 	// }

// 	// // Infinite iteration
// 	// while (1) {
// 	// 	surface.update();
// 	// 	// surface.displayWaterLevel();
// 	// 	surface.displayASCII();
// 	// 	std::cout << std::fixed <<"TotalWaterLevel = " << surface.getTotalWaterLevel() << std::endl;
// 	// 	frequencyCounter();
// 	// 	usleep(250000);
// 	// 	system("clear");
// 	// }

// }
