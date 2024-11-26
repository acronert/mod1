#include "Simulation.class.hpp"

Simulation::Simulation()
{
	_renderer = new Renderer();
	_waterSurface = nullptr;
	_window = nullptr;

	_rain_intensity = 0;
	_rise_intensity = 0;
	_wave_intensity = 0;
}

Simulation::~Simulation()
{
	delete _waterSurface;
	if (_renderer)
		delete _renderer;

	if (_window) {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

void resize_callback(GLFWwindow* window, int width, int height) {
	const float targetAspect = 4.0f / 3.0f;

	int viewportWidth, viewportHeight;
	if (width / (float)height < targetAspect) {
		viewportHeight = height;
		viewportWidth = static_cast<int>(height * targetAspect);
	} else {
		viewportWidth = width;
		viewportHeight = static_cast<int>(width / targetAspect);
	}

	int viewportX = (width - viewportWidth) / 2;
	int viewportY = (height - viewportHeight) / 2;

	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, targetAspect, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	(void)window;
}

void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	s_input* input = (s_input*)glfwGetWindowUserPointer(window); // get the input ptr

	input->scroll = yoffset;
	(void)xoffset;
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
		else if (key == GLFW_KEY_E)
			input->up = true;
		else if (key == GLFW_KEY_Q)
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
		else if (key == GLFW_KEY_DELETE)
			input->reset_water = true;
		else if (key == GLFW_KEY_SPACE)
			input->pause = !input->pause;
		else if (key == GLFW_KEY_1)
			input->rise_mode = true;
		else if (key == GLFW_KEY_2)
			input->rain_mode = true;
		else if (key == GLFW_KEY_3)
			input->wave_mode = true;

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
		else if (key == GLFW_KEY_E)
			input->up = false;
		else if (key == GLFW_KEY_Q)
			input->down = false;
		else if (key == GLFW_KEY_UP)
			input->pitchUp = false;
		else if (key == GLFW_KEY_DOWN)
			input->pitchDown = false;
		else if (key == GLFW_KEY_LEFT)
			input->yawLeft = false;
		else if (key == GLFW_KEY_RIGHT)
			input->yawRight = false;

		else if (key == GLFW_KEY_1)
			input->rise_mode = false;
		else if (key == GLFW_KEY_2)
			input->rain_mode = false;
		else if (key == GLFW_KEY_3)
			input->wave_mode = false;
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

void	Simulation::initializeCamera(int size) {
	// init camera
	_camera.posX = -size / 8;
	_camera.posY = -size / 8;
	_camera.posZ = (size + size) / 8;
	_camera.pitch = -35;
	_camera.yaw = 45;
}

void	Simulation::initializeWaterSurface(std::vector<float> heightMap, int size) {
	_waterSurface = new WaterSurface(size, size);
	if (heightMap.size() > static_cast<unsigned long>(size * size))
		throw std::invalid_argument("heightMap is larger than expected");
	for (size_t i = 0; i < heightMap.size(); i++)
		_waterSurface->setGroundLevel(i % size, i / size, heightMap[i]);
	_waterSurface->updateGroundNormal();
}

void	Simulation::waterControl() {

	if (_input.rise_mode && _input.scroll) {
		_rise_intensity = std::max(0, std::min(_rise_intensity + _input.scroll, 10));
		std::cout << "rise intensity = " << _rise_intensity << std::endl;
	}
	if (_input.rain_mode && _input.scroll) {
		_rain_intensity = std::max(0, std::min(_rain_intensity + _input.scroll, 10));
		std::cout << "rain intensity = " << _rain_intensity << std::endl;
	}
	if (_input.wave_mode && _input.scroll) {
		_wave_intensity = std::max(0, std::min(_wave_intensity + _input.scroll, 10));
		std::cout << "wave intensity = " << _wave_intensity << std::endl;
	}

	if (_input.reset_water) {
		_waterSurface->resetWater();
		_rise_intensity = 0;
		_rain_intensity = 0;
		_wave_intensity = 0;
		_input.reset_water = false;
	}

	if (_rise_intensity)
		_waterSurface->riseWater(_rise_intensity * 0.003f, 0.1f);
	if (_rain_intensity)
		_waterSurface->makeRain(_rain_intensity * 0.00005f, 1.5f);
	if (_wave_intensity)
		_waterSurface->makeWave(_wave_intensity * 0.45f);
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
	glfwSetScrollCallback(_window, scroll_callback);

	initializeCamera(size);

	while (!glfwWindowShouldClose(_window)) {
		glClear(GL_COLOR_BUFFER_BIT); // clear buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		_input.scroll = 0;
		glfwPollEvents(); // Poll for and process events (resize, kyboard, etc)
		_camera.update(_input);
		_renderer->render(*_waterSurface, _camera);
		if (!_input.pause) {
			waterControl();
			_waterSurface->update();
		}
		glfwSwapBuffers(_window); // Swap front and back buffers
		frequencyCounter();
	}
}