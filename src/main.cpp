#include "WaterSurface.class.hpp"
#include "Renderer2D.class.hpp"

#define WIDTH 5
#define HEIGHT 5
#define ITER 1000000

int	main() {

	WaterSurface surface(WIDTH, HEIGHT);

	surface.setHeight(2, 2, 1.0f);
	surface.displayHeight();
	for (int i = 0; i < ITER; i++) {
		surface.update();
		surface.displayHeight();
	}

	// for (int x = 0; x < WIDTH; x++) {
	// 	for (int y = 0; y < HEIGHT; y++) {
	// 		surface.setCellHeight(x, y, 1);
	// 	}
	// }
	// surface.setCellHeight(10, 7, 2);

	// surface.displayHeight();


	// Renderer2D renderer(WIDTH, HEIGHT);


	// while (1) {
	// 	surface.update();
	// 	renderer.update(surface.getCells());
	// 	std::cout << surface.getSumHeight() << std::endl;
	// 	// surface.displayHeight();
	// 	renderer.displayASCII();
	// 	usleep(250000); // 0.25sec
	// 	system("clear");
	// }


}
