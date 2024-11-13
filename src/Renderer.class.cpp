#include "Renderer.class.hpp"

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
	Controls* controls = (Controls*)glfwGetWindowUserPointer(window); // get the controls ptr

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_UP)
			controls->forward = true;
		else if (key == GLFW_KEY_DOWN)
			controls->backward = true;
		else if (key == GLFW_KEY_LEFT)
			controls->left = true;
		else if (key == GLFW_KEY_RIGHT)
			controls->right = true;
		else if (key == GLFW_KEY_SPACE)
			controls->up = true;
		else if (key == GLFW_KEY_LEFT_SHIFT)
			controls->down = true;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_UP)
			controls->forward = false;
		else if (key == GLFW_KEY_DOWN)
			controls->backward = false;
		else if (key == GLFW_KEY_LEFT)
			controls->left = false;
		else if (key == GLFW_KEY_RIGHT)
			controls->right = false;
		else if (key == GLFW_KEY_SPACE)
			controls->up = false;
		else if (key == GLFW_KEY_LEFT_SHIFT)
			controls->down = false;
	}
	std::cout << "Key = " << key << " / Action = " << action << std::endl;

	(void)scancode;
	(void)mods;

}


Renderer::Renderer() {
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
	glfwSetWindowUserPointer(_window, &_controls);

	// set callbacks
	glfwSetFramebufferSizeCallback(_window, resize_callback); // resize callback
	glfwSetKeyCallback(_window, key_callback);

	initializeGL();
}

Renderer::~Renderer() {
	if (_window) {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

Renderer::Renderer(const Renderer& other) {
	(void)other;
}

Renderer& Renderer::operator=(const Renderer& other) {
	if (this != &other) {}
	return *this;
}



void	Renderer::initializeGL() {
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

void	Renderer::setupCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(_camera.posX, _camera.posY, _camera.posZ,	// camera position
				_camera.targetX, _camera.targetY, _camera.targetZ,	// target position
				0.0 , 0.0, 1.0);		// Up direction
}

void	Renderer::initCamera() {
	_camera.posX = -_sizeX / 2;
	_camera.posY = - _sizeY / 2;
	_camera.posZ = (_sizeX + _sizeY) / 2;
	_camera.targetX = _sizeX / 2;
	_camera.targetY = _sizeY / 2;
	_camera.targetZ = 0;
}

void	Renderer::drawGroundVertices() {
	glBegin(GL_TRIANGLES);

	bool color = 0; // to alternate between triangle types
	for (size_t i = 0; i < _groundVertices.size(); i++) {
		if (i % 3 == 0)
			color = !color;
		glColor4f(color ? 1.0 : 0.5, 0.0, 0.0, 1.0); // RGBA
		glVertex3f(_groundVertices[i].x, _groundVertices[i].y, _groundVertices[i].z);
	}

	glEnd();
}

void	Renderer::drawWaterVertices() {
	glEnable(GL_BLEND);
	glBegin(GL_TRIANGLES);

	bool color = 0; // to alternate between triangle types
	for (size_t i = 0; i < _waterVertices.size(); i++) {
		if (i % 3 == 0)
			color = !color;
		glColor4f(0.0, 0.0, color ? 1.0 : 0.5, 0.8); // RGBA
		glVertex3f(_waterVertices[i].x, _waterVertices[i].y, _waterVertices[i].z);
	}

	glEnd();
}

// first triangle
//      3
//     "|\"
//     "| \"
//     "|  \"
//     "|___\"
//     1     2

// second triangle
//     3____2
//      \   |
//       \  |
//        \ |
//         \|
//          1
void	Renderer::generateVertices(std::vector<Cell>& cells) {
	float	h_coef = 2.0f;	// height coef
	
	_waterVertices.clear();
	_groundVertices.clear();
	for (int y = 0; y < _sizeY - 1; y++) {
		for (int x = 0; x < _sizeX - 1; x++) {
			// Water Vertices
				// First triangle of the quad
			_waterVertices.push_back({static_cast<float>(x), static_cast<float>(y), h_coef * cells[index(x, y)].getTotalLevel()});
			_waterVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y), h_coef * cells[index(x+1, y)].getTotalLevel()});
			_waterVertices.push_back({static_cast<float>(x), static_cast<float>(y + 1), h_coef * cells[index(x, y+1)].getTotalLevel()});
				// Second triangle of the quad
			_waterVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y), h_coef * cells[index(x+1, y)].getTotalLevel()});
			_waterVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y + 1), h_coef * cells[index(x+1, y+1)].getTotalLevel()});
			_waterVertices.push_back({static_cast<float>(x), static_cast<float>(y + 1), h_coef * cells[index(x, y+1)].getTotalLevel()});

			// Ground Vertices
				// First triangle of the quad
			_groundVertices.push_back({static_cast<float>(x), static_cast<float>(y), h_coef * cells[index(x, y)].getGroundLevel()});
			_groundVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y), h_coef * cells[index(x+1, y)].getGroundLevel()});
			_groundVertices.push_back({static_cast<float>(x), static_cast<float>(y + 1), h_coef * cells[index(x, y)].getGroundLevel()});
				// Second triangle of the quad
			_groundVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y), h_coef * cells[index(x+1, y)].getGroundLevel()});
			_groundVertices.push_back({static_cast<float>(x + 1), static_cast<float>(y + 1), h_coef * cells[index(x+1, +1)].getGroundLevel()});
			_groundVertices.push_back({static_cast<float>(x), static_cast<float>(y + 1), h_coef * cells[index(x, y+1)].getGroundLevel()});
		}
	}
}


int	Renderer::index(int x, int y) {
	return x + y * _sizeX;
}


// void	Renderer::manageInput() {

// }

void	Renderer::render(WaterSurface& surface) {

	_sizeX = surface.getSizeX();
	_sizeY = surface.getSizeY();

	initCamera();

	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents(); // Poll for and process events (resize, kyboard, etc)

		// manageInput();

		// clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		setupCamera();


		// generate and draw vertices
		generateVertices(surface.getCells());
		drawGroundVertices();
		drawWaterVertices();

		surface.update();

		glfwSwapBuffers(_window); // Swap front and back buffers

		frequencyCounter();
	}

}