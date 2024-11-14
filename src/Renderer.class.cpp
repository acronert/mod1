#include "Renderer.class.hpp"

Renderer::Renderer() {
	// _waterTexture = loadTexture(WATER_TEXTURE_PATH);

	// _waterShader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
}

Renderer::~Renderer() {}

// GLuint	Renderer::loadTexture(const char* filename) {
// 	GLuint	texture;
// 	glGenTextures(1, &texture);				// generate a unique texture ID and store it in texture
// 	glBindTexture(GL_TEXTURE_2D, texture);	// Bind the texture ID to the current GL_TEXTURE_2D (will be used until unbound)

// 	int width;
// 	int height;
// 	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
// 	if (!image) {
// 		std::cerr << "Failed to load texture\n";
// 		return 0;
// 	}
// 	// load the texture in the image
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

// 	glGenerateMipmap(GL_TEXTURE_2D); // Create mipmaps for texture

// 	// Texture settings
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// 	SOIL_free_image_data(image);
// 	glBindTexture(GL_TEXTURE_2D, 0); // unbound the current texture and reset GL_TEXTURE_2d
// 	return texture;
// }

// SHADERS /////////////////////////////////////////
	// load shader files
		// vertex shader: transform vertices
		// fragment shader : compute color of the pixels
	// compile it
	// link the shaders
	// use them by loading them before drawing vertices

// GLuint Renderer::loadShader(const char* filepath, GLenum shaderType) {
// 	// Read shader file
// 	std::ifstream file(filepath);
// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string code = buffer.str();
// 	const char* shaderCode = code.c_str();

// 	// Create and compile shader
// 	GLuint shader = glCreateShader(shaderType);
// 	glShaderSource(shader, 1, &shaderCode, NULL);
// 	glCompileShader(shader);

// 	// Check for compilation errors
// 	GLint success;
// 	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
// 	if (!success) {
// 		char infoLog[512];
// 		glGetShaderInfoLog(shader, 512, NULL, infoLog);
// 		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}
// 	return shader;
// }

// GLuint Renderer::createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath) {
// 	GLuint vertexShader = loadShader(vertexFilePath, GL_VERTEX_SHADER);
// 	GLuint fragmentShader = loadShader(fragmentFilePath, GL_FRAGMENT_SHADER);

// 	GLuint shaderProgram = glCreateProgram();
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);
// 	glLinkProgram(shaderProgram);

// 	// Check for linking errors
// 	GLint success;
// 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// 	if (!success) {
// 		char infoLog[512];
// 		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
// 		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
// 	}

// 	// Clean up
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

// 	return shaderProgram;
// }
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
	glEnable(GL_BLEND);			// Blend for transparency
	// glEnable(GL_TEXTURE_2D);	// Enable Texutre
	// glBindTexture(GL_TEXTURE_2D, _waterTexture);

	glBegin(GL_TRIANGLES);

	bool color = 0; // to alternate between triangle types
	for (size_t i = 0; i < _waterVertices.size(); i++) {
		if (i % 3 == 0)
			color = !color;

		// if depth < 1, the water is more transparent
		// float	transparency = 0.7 * min(1.0f, cells.)
		// glColor4f(0.0, 0.0, color ? 1.0 : 0.5, transparency); // RGBA
		glColor4f(0.0, 0.0, color ? 1.0 : 0.5, 0.7); // RGBA
		glVertex3f(_waterVertices[i].x, _waterVertices[i].y, _waterVertices[i].z);
	}

	glEnd();

	// glBindTexture(GL_TEXTURE_2D, 0);	// unbind texture
	// glDisable(GL_TEXTURE_2D);			// disable texturing
}

// first triangle   second triangle
//      3               3___2      
//     "|\"              \  |      
//     "| \"              \ |      
//     "|__\"              \|      
//     1    2               1      

void Renderer::generateVertices(std::vector<Cell>& cells) {
	int h_coef = 1;
	_waterVertices.clear();
	_groundVertices.clear();
	_waterVertices.reserve((_sizeX - 1) * (_sizeY - 1) * 6);
	_groundVertices.reserve((_sizeX - 1) * (_sizeY - 1) * 6);

	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Generate water vertices
			const float vertices[4][3] = {
				{static_cast<float>(x), static_cast<float>(y), 
				h_coef * cells[index(x, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y),
				h_coef * cells[index(x + 1, y)].getWaterVertexHeight()},
				{static_cast<float>(x), static_cast<float>(y + 1),
				h_coef * cells[index(x, y + 1)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),
				h_coef * cells[index(x + 1, y + 1)].getWaterVertexHeight()}
			};

			// First triangle
			_waterVertices.push_back({vertices[0][0], vertices[0][1], vertices[0][2]});
			_waterVertices.push_back({vertices[1][0], vertices[1][1], vertices[1][2]});
			_waterVertices.push_back({vertices[2][0], vertices[2][1], vertices[2][2]});

			// Second triangle
			_waterVertices.push_back({vertices[1][0], vertices[1][1], vertices[1][2]});
			_waterVertices.push_back({vertices[3][0], vertices[3][1], vertices[3][2]});
			_waterVertices.push_back({vertices[2][0], vertices[2][1], vertices[2][2]});

			// Generate ground vertices
			const float groundVertices[4][3] = {
				{static_cast<float>(x), static_cast<float>(y),
				h_coef * cells[index(x, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y),
				h_coef * cells[index(x + 1, y)].getGroundLevel()},
				{static_cast<float>(x), static_cast<float>(y + 1),
				h_coef * cells[index(x, y + 1)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),
				h_coef * cells[index(x + 1, y + 1)].getGroundLevel()}
			};

			// First triangle
			_groundVertices.push_back({groundVertices[0][0], groundVertices[0][1], groundVertices[0][2]});
			_groundVertices.push_back({groundVertices[1][0], groundVertices[1][1], groundVertices[1][2]});
			_groundVertices.push_back({groundVertices[2][0], groundVertices[2][1], groundVertices[2][2]});

			// Second triangle
			_groundVertices.push_back({groundVertices[1][0], groundVertices[1][1], groundVertices[1][2]});
			_groundVertices.push_back({groundVertices[3][0], groundVertices[3][1], groundVertices[3][2]});
			_groundVertices.push_back({groundVertices[2][0], groundVertices[2][1], groundVertices[2][2]});
		}
	}
}
int	Renderer::index(int x, int y) {
	return x + y * _sizeX;
}

void	Renderer::render(WaterSurface& surface, Camera& camera) {
	_sizeX = surface.getSizeX();
	_sizeY = surface.getSizeY();

	setupCamera(camera);

	generateVertices(surface.getCells());
	drawGroundVertices();

	// glUseProgram(_waterShader);
	drawWaterVertices();
	// glUseProgram(0);
}