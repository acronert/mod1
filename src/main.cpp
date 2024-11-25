#include "Simulation.class.hpp"
#include "MapGenerator.class.hpp"

#define SIZE	300

int	main(int argc, char** argv) {

	try {
		Simulation simulation;
		std::vector<float>	map;

		if (argc == 2) {
			MapGenerator mapGenerator(std::string(argv[1]), SIZE);
			map = mapGenerator.getMap();
		}
		
		simulation.run(map, SIZE);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
}
