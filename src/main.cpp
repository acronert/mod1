#include "WaterSurface.class.hpp"

int	main() {

	WaterSurface surface(20, 20, 1);

	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			surface.setCellHeight(x, y, 5);
		}
	}

	surface.setCellHeight(1, 1, 8);
	surface.setCellHeight(15, 15, 8);


	for (int i = 0; i < 200000; i++) {
		surface.computeWaterMovement();
		std::cout << "sum = " << surface.getHeightSum() << std::endl;
		surface.displayHeight();
		usleep(500000);
	}
}
