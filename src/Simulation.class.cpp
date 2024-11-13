#include "Simulation.class.hpp"

Simulation::Simulation()
{
	_waterSurface = new WaterSurface(WIDTH, HEIGHT);
	_renderer = new Renderer();
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

void	Simulation::init() {
	// parse and create the ground map
	for (int i = HEIGHT / 5; i < 4 * HEIGHT / 5; i++) {
		_waterSurface->setGroundLevel(WIDTH/3 -2, i, 2.0f);
		_waterSurface->setGroundLevel(WIDTH/3 -1, i, 2.5f);
		_waterSurface->setGroundLevel(WIDTH/3, i, 3.0f);
		_waterSurface->setGroundLevel(WIDTH/3 + 1, i, 2.5f);
		_waterSurface->setGroundLevel(WIDTH/3 + 2, i, 2.0f);

		_waterSurface->setGroundLevel(2 * WIDTH/3 -2, i, 3.0f);
		_waterSurface->setGroundLevel(2 * WIDTH/3 -1, i, 3.0f);
		_waterSurface->setGroundLevel(2 * WIDTH/3, i, 3.0f);
		_waterSurface->setGroundLevel(2 * WIDTH/3 + 1, i, 3.0f);
		_waterSurface->setGroundLevel(2 * WIDTH/3 + 2, i, 3.0f);
	}
	for (int i = 0; i < WIDTH / 5; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			_waterSurface->setGroundLevel(i, j, (WIDTH / 20) - (float)i / 4);
		}
	}

	// create the water surface / set the type of water event (rain, rise, wave)
	_waterSurface->setWaterLevel(WIDTH / 2, HEIGHT / 2, 1000);

	initializeGL();
	initializeCamera();
}

void	Simulation::initializeGL() {
	// Initialize window
	if (!glfwInit()) {
		throw std::runtime_error("Failed to init GLFW");
	}

	_window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "mod1", NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(_window);

	// set window user pointer
	glfwSetWindowUserPointer(_window, &_input);

	// set callbacks
	glfwSetFramebufferSizeCallback(_window, resize_callback); // resize callback
	glfwSetKeyCallback(_window, key_callback);

	// set perspective
	glEnable(GL_DEPTH_TEST);	// Enable 3D rendering
	glEnable(GL_MULTISAMPLE);	// MSAA (MultiSample Anti-Aliasing)
	glEnable(GL_BLEND);			// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// set blending for transparency

	// set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
}

void	Simulation::initializeCamera() {
	// init camera
	_camera.posX = -WIDTH / 2;
	_camera.posY = -HEIGHT / 2;
	_camera.posZ = (WIDTH + HEIGHT) / 2;
}

void	Simulation::run() {

	init();

	while (!glfwWindowShouldClose(_window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffer

		glfwPollEvents(); // Poll for and process events (resize, kyboard, etc)
		_camera.update(_input);

		_renderer->render(*_waterSurface, _camera);
		_waterSurface->update();
		glfwSwapBuffers(_window); // Swap front and back buffers

		frequencyCounter();
	}
}