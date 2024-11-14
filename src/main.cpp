#include "Simulation.class.hpp"
#include "MapGenerator.class.hpp"

#define SIZE	100

int	main(int argc, char** argv) {


	// Simulation simulation;
	// simulation.run();
	// (void)argv;
	// (void)argc;
	try {

		Simulation simulation;
		std::vector<float>	map;

		if (argc == 2) {
			MapGenerator mapGenerator(std::string(argv[1]), SIZE);
			map = mapGenerator.getMap();
			std::cout << "map total size = " << map.size() << std::endl;
		}

		simulation.run(map, SIZE);

	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
}
