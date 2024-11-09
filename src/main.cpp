#include "WaterSurface.class.hpp"

int	main() {

	WaterSurface surface(10, 10, 1);

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			surface.setCellHeight(x, y, 50);
		}
	}

	surface.setCellHeight(5, 5, 55);


	for (int i = 0; i < 5; i++) {
		surface.displayHeight();
		surface.computeWaterMovement();
	}
}
