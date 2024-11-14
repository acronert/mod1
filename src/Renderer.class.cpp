#include "Renderer.class.hpp"

Renderer::Renderer() {

}

Renderer::~Renderer() {}


// SHADERS /////////////////////////////////////////
	// load shader files
	// 	vertex shader: transform vertices
	// 	fragment shader : compute color of the pixels
	// compile it
	// link the shaders
	// use them by loading them before drawing vertices

void	Renderer::initShaders() {
	_water_shader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
}

GLuint	Renderer::loadShader(const char* filepath, GLenum shaderType) {
	// Read shader file
	std::ifstream file(filepath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string code = buffer.str();
	const char* shaderCode = code.c_str();
	// Create and compile shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// Check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

GLuint	Renderer::createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath) {
	GLuint vertexShader = loadShader(vertexFilePath, GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShader(fragmentFilePath, GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Clean up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
////////////////////////////////////////////////////

void Renderer::setupCamera(Camera& camera) {
	float yawRad = camera.yaw * M_PI / 180.0f;
	float pitchRad = camera.pitch * M_PI / 180.0f;

	float dirX = cos(pitchRad) * cos(yawRad);
	float dirY = cos(pitchRad) * sin(yawRad);
	float dirZ = sin(pitchRad);

	float targetX = camera.posX + dirX;
	float targetY = camera.posY + dirY;
	float targetZ = camera.posZ + dirZ;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.posX, camera.posY, camera.posZ,  // Camera position
			targetX, targetY, targetZ,              // Target position
			0.0, 0.0, 1.0);                        // Up direction
}


void	Renderer::drawGroundVertices(std::vector<Cell>& cells) {
	glBegin(GL_TRIANGLES);

	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Generate water vertices
			const float vertices[4][3] = {
				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getGroundLevel()},
				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getGroundLevel()},
			};

			// first triangle
				glColor4f(0.2, 0.7, 0.2, 1.0);
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
				glColor4f(0.2, 0.7, 0.2, 1.0);
				glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
				glColor4f(0.2, 0.7, 0.2, 1.0);
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

			// second triangle
				glColor4f(0.2, 0.5, 0.2, 1.0);
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
				glColor4f(0.2, 0.5, 0.2, 1.0);
				glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
				glColor4f(0.2, 0.5, 0.2, 1.0);
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
		}
	}
	glEnd();
}

void	Renderer::drawWaterVertices(std::vector<Cell>& cells) {

	// glUseProgram(_water_shader); // activate shader

	glEnable(GL_BLEND);			// Blend for transparency

	glBegin(GL_TRIANGLES);

	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Generate water vertices
			const float vertices[4][3] = {
				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getWaterVertexHeight()},
				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getWaterVertexHeight()},
			};
			const float waterLevel[4] = {
				cells[index(x, y)].getWaterLevel(),
				cells[index(x + 1, y)].getWaterLevel(),
				cells[index(x + 1, y + 1)].getWaterLevel(),
				cells[index(x, y + 1)].getWaterLevel()
			};

			// first triangle
				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[0] / 2.0f));
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[1] / 2.0f));
				glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[2] / 2.0f));
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

			// second triangle
				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[2] / 2.0f));
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[3] / 2.0f));
				glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[0] / 2.0f));
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
		}
	}
	glEnd();
	// glUseProgram(0);	// deactive shader
}

int	Renderer::index(int x, int y) {
	return x + y * _sizeX;
}

void	Renderer::render(WaterSurface& surface, Camera& camera) {
	_sizeX = surface.getSizeX();
	_sizeY = surface.getSizeY();

	setupCamera(camera);

	drawGroundVertices(surface.getCells());
	drawWaterVertices(surface.getCells());
	
}