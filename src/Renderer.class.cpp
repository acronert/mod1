#include "Renderer.class.hpp"



void printMat4(const glm::mat4& mat) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

Renderer::Renderer() {

}

Renderer::~Renderer() {}


GLint	Renderer::loadShader(const char* filepath, GLenum shaderType) {
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

GLint	Renderer::createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath) {
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

	glm::vec3	cameraPos(camera.posX, camera.posY, camera.posZ);
	glm::vec3	cameraTarget(targetX, targetY, targetZ);
	glm::vec3	up(0.0f, 0.0f, 1.0f);

	_view = glm::lookAt(cameraPos, cameraTarget, up);

	glUseProgram(_water_shader);
	GLint	viewLoc = glGetUniformLocation(_water_shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_view));
	if (viewLoc == -1)
		std::cerr << "Error: viewLoc uniform location is invalid (in setupCamera)" << std::endl;
	glUseProgram(0);

	glUseProgram(_ground_shader);
	viewLoc = glGetUniformLocation(_ground_shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_view));
	if (viewLoc == -1)
		std::cerr << "Error: viewLoc uniform location is invalid (in setupCamera)" << std::endl;
	glUseProgram(0);
}

void	Renderer::pushQuadVertex(s_vec3 quad, s_vec3 color, std::vector<float>* vertices) {
	vertices->push_back(quad.x);
	vertices->push_back(quad.y);
	vertices->push_back(quad.z);
	vertices->push_back(color.x);	// r
	vertices->push_back(color.y);	// g
	vertices->push_back(color.z);	// b
}

std::vector<float>	Renderer::createWaterVertices(std::vector<Cell>& cells) {

	// will contain vertex positions, heights and colors (and alpha ?)
	//	x1, y1, height1, r1, g1, b1,
	//	x2, y2, height2, r2, g2, b2,
	// ...
	std::vector<float>	vertices;

	// FILL THE VERTICES ////////////
	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Find quads	(SW, SE, NE, NW)
			const s_vec3 quad[4] = {
				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getWaterVertexHeight()},
				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getWaterVertexHeight()},
			};

			// first triangle : 0 -> 1 -> 2
			s_vec3 color = {0.0, 0.0, 0.7};
			pushQuadVertex(quad[0], color, &vertices);
			pushQuadVertex(quad[1], color, &vertices);
			pushQuadVertex(quad[2], color, &vertices);

			// second triangle : 2 -> 3 -> 0
			color = {0.0, 0.0, 0.5};
			pushQuadVertex(quad[2], color, &vertices);
			pushQuadVertex(quad[3], color, &vertices);
			pushQuadVertex(quad[0], color, &vertices);
		}
	}
	return vertices;
}

std::vector<float>	Renderer::createGroundVertices(std::vector<Cell>& cells) {

	// will contain vertex positions, heights and colors (and alpha ?)
	//	x1, y1, height1, r1, g1, b1,
	//	x2, y2, height2, r2, g2, b2,
	// ...
	std::vector<float>	vertices;

	// FILL THE VERTICES ////////////
	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Find quads	(SW, SE, NE, NW)
			const s_vec3 quad[4] = {
				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getGroundLevel()},
				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getGroundLevel()},
			};

			// first triangle : 0 -> 1 -> 2
			s_vec3 color = {0.0, 0.7, 0.0};
			pushQuadVertex(quad[0], color, &vertices);
			pushQuadVertex(quad[1], color, &vertices);
			pushQuadVertex(quad[2], color, &vertices);

			// second triangle : 2 -> 3 -> 0
			color = {0.0, 0.5, 0.0};
			pushQuadVertex(quad[2], color, &vertices);
			pushQuadVertex(quad[3], color, &vertices);
			pushQuadVertex(quad[0], color, &vertices);
		}
	}
	return vertices;
}

int	Renderer::index(int x, int y) {
	return x + y * _sizeX;
}

void	Renderer::initializeShader(GLint shader, GLuint VAO, GLuint VBO) {

	glUseProgram(shader);
	// Bind VAO
	glBindVertexArray(VAO);
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Define VBO attributes (no data yet)
		// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	// set the location (here to 0)
		// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setup Shader Uniforms (projection, view and model)
			// glUniformMatrix4fv(location of the uniform variable,
						// nbr of matrices,
						// transpose,
						// ptr to the data)
		// Projection
	GLint	projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(_projection));
	if (projectionLoc == -1)
		std::cerr << "Error: projectionLoc uniform location is invalid" << std::endl;
		// View
	GLint	viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_view));
	if (viewLoc == -1)
		std::cerr << "Error: viewLoc uniform location is invalid" << std::endl;
		// Model
	GLint	modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_model));
	if (modelLoc == -1)
		std::cerr << "Error: modelLoc uniform location is invalid" << std::endl;
}

void	Renderer::init() {
	// Generate _projection matrix
	float	aspectRatio = 4.0f/3.0f;
	_projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.01f, 1000.0f);

	// Generate _view matrix
	glm::vec3	cameraPos(10.0f, 10.0f, 10.0f);
	glm::vec3	cameraTarget(0.0f, .0f, 0.0f);
	glm::vec3	up(0.0f, 1.0f, 0.0f);
	_view = glm::lookAt(cameraPos, cameraTarget, up);

	// Generate _model matrix
	_model = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);							// Enable 3D rendering
	// glEnable(GL_MULTISAMPLE);							// MSAA (MultiSample Anti-Aliasing)
	// glEnable(GL_BLEND);									// enable blending
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// set blending for transparency

	// Initialize Shaders
	_water_shader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
	_ground_shader = createShaderProgram(GROUND_VERTEX_SHADER, GROUND_FRAGMENT_SHADER);

	// Generate VBO and VAO
	glGenBuffers(1, &_waterVBO);
	glGenVertexArrays(1, &_waterVAO);
	glGenBuffers(1, &_groundVBO);
	glGenVertexArrays(1, &_groundVAO);

	initializeShader(_water_shader, _waterVAO, _waterVBO);
	initializeShader(_ground_shader, _groundVAO, _groundVBO);
}

void	Renderer::renderLayer(std::vector<float> vertices, GLint shader, GLuint VAO, GLuint VBO) {
	glUseProgram(shader);
	// Update the VBO with the new vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		// Define vertex attributes
			// void glVertexAttribPointer(
			//     GLuint index,          // Attribute index in the shader (layout location)
			//     GLint size,            // Number of components per vertex attribute
			//     GLenum type,           // Data type of each component
			//     GLboolean normalized,  // Whether to normalize the data
			//     GLsizei stride,        // Byte offset between consecutive attributes
			//     const void *pointer    // Byte offset to the first component of the attribute in the VBO
			// );

	// Bind the VAO
	glBindVertexArray(VAO);
	// Draw the vertices
	int vertexCount = (_sizeX - 1) * (_sizeY - 1) * 6;
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	// Unbind the VAO
	glBindVertexArray(0);
	glUseProgram(0);
}

void	Renderer::render(WaterSurface& surface, Camera& camera) {
	_sizeX = surface.getSizeX();
	_sizeY = surface.getSizeY();

	setupCamera(camera);

	// Create the vertices
	std::vector<float> water_vertices = createWaterVertices(surface.getCells());
	std::vector<float> ground_vertices = createGroundVertices(surface.getCells());

	renderLayer(water_vertices, _water_shader, _waterVAO, _waterVBO);
	renderLayer(ground_vertices, _ground_shader, _groundVAO, _groundVBO);


	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
		// Handle the error
	}
}















// void	Renderer::drawWaterVertices(std::vector<Cell>& cells) {

// 	// glUseProgram(_water_shader); // activate shader

// 	glEnable(GL_BLEND);			// Blend for transparency

// 	glBegin(GL_TRIANGLES);

// 	for (int y = 0; y < _sizeY - 1; ++y) {
// 		for (int x = 0; x < _sizeX - 1; ++x) {
// 			// Generate water vertices
// 			const float vertices[4][3] = {
// 				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getWaterVertexHeight()},
// 				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getWaterVertexHeight()},
// 				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getWaterVertexHeight()},
// 				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getWaterVertexHeight()},
// 			};
// 			const float waterLevel[4] = {
// 				cells[index(x, y)].getWaterLevel(),
// 				cells[index(x + 1, y)].getWaterLevel(),
// 				cells[index(x + 1, y + 1)].getWaterLevel(),
// 				cells[index(x, y + 1)].getWaterLevel()
// 			};

// 			// first triangle
// 				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[0] / 2.0f));
// 				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
// 				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[1] / 2.0f));
// 				glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
// 				glColor4f(0.0, 0.0, 0.8, 0.7 * std::min(1.0f, waterLevel[2] / 2.0f));
// 				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

// 			// second triangle
// 				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[2] / 2.0f));
// 				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
// 				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[3] / 2.0f));
// 				glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
// 				glColor4f(0.0, 0.0, 0.6, 0.7 * std::min(1.0f, waterLevel[0] / 2.0f));
// 				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
// 		}
// 	}
// 	glEnd();
// 	// glUseProgram(0);	// deactive shader
// }

// void	Renderer::drawGroundVertices(std::vector<Cell>& cells) {
// 	glBegin(GL_TRIANGLES);

// 	for (int y = 0; y < _sizeY - 1; ++y) {
// 		for (int x = 0; x < _sizeX - 1; ++x) {
// 			// Generate water vertices
// 			const float vertices[4][3] = {
// 				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getGroundLevel()},
// 				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getGroundLevel()},
// 				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getGroundLevel()},
// 				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getGroundLevel()},
// 			};

// 			// first triangle
// 				glColor4f(0.2, 0.7, 0.2, 1.0);
// 				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
// 				glColor4f(0.2, 0.7, 0.2, 1.0);
// 				glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
// 				glColor4f(0.2, 0.7, 0.2, 1.0);
// 				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

// 			// second triangle
// 				glColor4f(0.2, 0.5, 0.2, 1.0);
// 				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
// 				glColor4f(0.2, 0.5, 0.2, 1.0);
// 				glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
// 				glColor4f(0.2, 0.5, 0.2, 1.0);
// 				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
// 		}
// 	}
// 	glEnd();
// }
