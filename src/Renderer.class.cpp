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
	glEnable(GL_DEPTH_TEST); // Enable 3D rendering

	// resize callback
	glfwSetFramebufferSizeCallback(_window, resize_callback);

	// set perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
}

Renderer::~Renderer() {}

Renderer::Renderer(const Renderer& other) {
	(void)other;
}

Renderer& Renderer::operator=(const Renderer& other) {
	if (this != &other) {


	}
	return *this;
}

float angle = 0;

void	Renderer::render(WaterSurface& surface) {
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents(); // Poll for and process events (resize, kyboard, etc)


		// clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set camerar position
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(200.0, -100.0, 100.0,	// camera position
					100.0, 100.0, 0.0,	// target position
					0.0, 0.0, 1.0);		// Up direction


		// generate and draw vertices
		generateVertices(surface.getCells(), surface.getSizeX(), surface.getSizeY());
		drawGroundVertices();
		drawWaterVertices();
		surface.update();

		glfwSwapBuffers(_window); // Swap front and back buffers

		frequencyCounter();
	}
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void	Renderer::drawGroundVertices() {
	glBegin(GL_TRIANGLES);

	int i = 0;
	int color = 0;
	for(std::vector<s_vertex>::iterator it = _groundVertices.begin(); it != _groundVertices.end(); it++) {
		if (i % 3 == 0)
			color = !color;
		color ? glColor3f(0.5, 0.0, 0.0): glColor3f(1.0, 0.0, 0.0);
		glVertex3f(it->x, it->y, it->z);
		i++;
	}

	glEnd();
}

void	Renderer::drawWaterVertices() {
	glBegin(GL_TRIANGLES);

	int i = 0;
	int color = 0;
	for(std::vector<s_vertex>::iterator it = _waterVertices.begin(); it != _waterVertices.end(); it++) {
		if (i % 3 == 0)
			color = !color;
		color ? glColor3f(0.0, 0.0, 1.0): glColor3f(0.0, 0.0, 0.5);
		glVertex3f(it->x, it->y, it->z);
		i++;
	}

	glEnd();
}

// first triangle
//      3
//      |\
//      | \
//      |  \
//      |___\
//     1     2

// second triangle
//     3____2
//      \   |
//       \  |
//        \ |
//         \|
//          1
void	Renderer::generateVertices(std::vector<Cell>& cells, int sizeX, int sizeY) {
	_sizeX = sizeX;
	_sizeY = sizeY;

	float	h_coef = 2.0f;	// height coef
	
	_waterVertices.clear();
	_groundVertices.clear();
	for (int y = 0; y < sizeY - 1; y++) {
		for (int x = 0; x < sizeX - 1; x++) {
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



