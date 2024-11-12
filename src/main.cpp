#include "WaterSurface.class.hpp"

#include <ctime>

#define WIDTH 100
#define HEIGHT 100
#define ITER 10

void	frequencyCounter() {
	static int count = 0;
	std::time_t start;

	if (count == 0)
		start = std::time(0);
	count++;

	std::time_t now = std::time(0);
	if (now - start >= 1) {
		std::cout << count << "Hz" << std::endl;
		count = 0;
		start = now;
	}
}

int	main() {

	WaterSurface surface(WIDTH, HEIGHT);



	for (int i = WIDTH / 5 ; i < 4 * WIDTH /5; i++) {
		surface.setGroundLevel(HEIGHT / 2, i, 1.5f);
		surface.setWaterLevel(HEIGHT / 2, i, 0.0f);
		surface.setWaterLevel(0, i, 15.0f);
	}
	surface.setWaterLevel(60, 30, 40.0f);

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
		system("clear");
		frequencyCounter();
	}

}

