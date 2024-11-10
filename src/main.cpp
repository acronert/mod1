#include "WaterSurface.class.hpp"
#include "Renderer2D.class.hpp"

#define WIDTH 20
#define HEIGHT 20

int	main() {

	WaterSurface surface(WIDTH, HEIGHT);

	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			surface.setCellHeight(x, y, 1);
		}
	}
	surface.setCellHeight(10, 7, 2);

	surface.displayHeight();


	Renderer2D renderer(WIDTH, HEIGHT);


	while (1) {
		surface.update();
		renderer.update(surface.getCells());
		std::cout << surface.getSumHeight() << std::endl;
		// surface.displayHeight();
		renderer.displayASCII();
		usleep(250000); // 0.25sec
		system("clear");
	}


}
