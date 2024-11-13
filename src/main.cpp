#include "Simulation.class.hpp"
#include "MapGenerator.class.hpp"

int	main(int argc, char** argv) {

	Simulation simulation;
	simulation.run();

	(void)argv;
	(void)argc;

	// if (argc != 2)
	// 	return 0;
	// try {
	// 	MapGenerator mapGenerator(std::string(argv[1]), 10);

	// }
	// catch (const std::exception& e) {
	// 	std::cout << e.what() << std::endl;
	// }
	
}
