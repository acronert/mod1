#include "Simulation.class.hpp"

Simulation::Simulation()
{
	_renderer = new Renderer();
	_waterSurface = nullptr;
	_window = nullptr;
}

Simulation::~Simulation()
{
	delete _waterSurface;
	delete _renderer;

	if (_window) {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

void	resize_callback(GLFWwindow* window, int width, int height) {
	// Update the viewport size
	glViewport(0, 0, width, height);

	// Update the projection matrix to maintain the aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	(void)window;
}

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	s_input* input = (s_input*)glfwGetWindowUserPointer(window); // get the input ptr

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W)
			input->forward = true;
		else if (key == GLFW_KEY_S)
			input->backward = true;
		else if (key == GLFW_KEY_A)
			input->left = true;
		else if (key == GLFW_KEY_D)
			input->right = true;
		else if (key == GLFW_KEY_SPACE)
			input->up = true;
		else if (key == GLFW_KEY_LEFT_SHIFT)
			input->down = true;
		else if (key == GLFW_KEY_UP)
			input->pitchUp = true;
		else if (key == GLFW_KEY_DOWN)
			input->pitchDown = true;
		else if (key == GLFW_KEY_LEFT)
			input->yawLeft = true;
		else if (key == GLFW_KEY_RIGHT)
			input->yawRight = true;

		// Simulation controls
		else if (key == GLFW_KEY_Q)
			input->reset_water = true;
		else if (key == GLFW_KEY_P)
			input->pause = !input->pause;
		else if (key == GLFW_KEY_1)
			input->rise_mode = !input->rise_mode;
		else if (key == GLFW_KEY_2)
			input->rain_mode = !input->rain_mode;
		else if (key == GLFW_KEY_3)
			input->wave_mode = !input->wave_mode;

	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W)
			input->forward = false;
		else if (key == GLFW_KEY_S)
			input->backward = false;
		else if (key == GLFW_KEY_A)
			input->left = false;
		else if (key == GLFW_KEY_D)
			input->right = false;
		else if (key == GLFW_KEY_SPACE)
			input->up = false;
		else if (key == GLFW_KEY_LEFT_SHIFT)
			input->down = false;
		else if (key == GLFW_KEY_UP)
			input->pitchUp = false;
		else if (key == GLFW_KEY_DOWN)
			input->pitchDown = false;
		else if (key == GLFW_KEY_LEFT)
			input->yawLeft = false;
		else if (key == GLFW_KEY_RIGHT)
			input->yawRight = false;

	}

	(void)scancode;
	(void)mods;
}


void	frequencyCounter() {
	static int count = 0;
	static std::time_t start = 0;

	if (count == 0)
		start = std::time(0);
	count++;

	std::time_t now = std::time(0);
	if (now - start >= 1) {
		std::cout << count << "FPS" << std::endl;
		count = 0;
		start = now;
	}
}

void	Simulation::initializeGL() {

}

void	Simulation::initializeCamera(int size) {
	// init camera
	_camera.posX = -size / 2;
	_camera.posY = -size / 2;
	_camera.posZ = (size + size) / 2;
	_camera.pitch = -35;
	_camera.yaw = 45;
}

void	Simulation::initializeWaterSurface(std::vector<float> heightMap, int size) {

	_waterSurface = new WaterSurface(size, size);

	std::cout << heightMap.size() << std::endl;

	if (heightMap.size() > size * size)
		throw std::invalid_argument("heightMap is larger than expected");

	for (size_t i = 0; i < heightMap.size(); i++) {
		_waterSurface->setGroundLevel(i % size, i /size, heightMap[i]);
	}
}

void	Simulation::waterControl() {
	if (_input.reset_water) {
		_waterSurface->resetWater();
		_input.reset_water = false;
	}

	if (_input.rise_mode) {
		_waterSurface->riseWater(0.01f, 0.1f);
	}

	if (_input.rain_mode) {
		_waterSurface->makeRain(0.0003f, 1.0f);
	}

	if (_input.wave_mode) {
		_waterSurface->makeWave(50.0f);
		_input.wave_mode = false;
	}
}

void	Simulation::run(std::vector<float> heightMap, int size) {

	if (size < 0)
		throw std::invalid_argument("invalid size");

	initializeWaterSurface(heightMap, size);

	// Initialize window
	if (!glfwInit())
		throw std::runtime_error("Failed to init GLFW");

	_window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "mod1", NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(_window);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");

	// initialize renderer
	_renderer->init(_waterSurface->getCells(), size);

	// set window user pointer
	glfwSetWindowUserPointer(_window, &_input);

	// set callbacks
	glfwSetFramebufferSizeCallback(_window, resize_callback); // resize callback
	glfwSetKeyCallback(_window, key_callback);

	initializeCamera(size);

	while (!glfwWindowShouldClose(_window)) {

		// double frameStartTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT); // clear buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		glfwPollEvents(); // Poll for and process events (resize, kyboard, etc)

		_camera.update(_input);

		_renderer->render(*_waterSurface, _camera);

		if (!_input.pause) {
			waterControl();
			_waterSurface->update();
		}

		glfwSwapBuffers(_window); // Swap front and back buffers

		frequencyCounter();

	// 	double frameEndTime = glfwGetTime();
	// 	double frameDuration = frameEndTime - frameStartTime;
	// 	double timeToWait = 1.0f / TARGET_FPS - frameDuration;
	// 	if (timeToWait > 0) {
	// 		glfwWaitEventsTimeout(timeToWait);
    // }
	}
}