#include "Renderer.class.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &_groundVAO);
	glDeleteVertexArrays(1, &_waterVAO);
	glDeleteBuffers(1, &_groundVBO);
	glDeleteBuffers(1, &_waterStaticVBO);
	glDeleteBuffers(1, &_waterDynamicVBO);
	glDeleteProgram(_ground_shader);
	glDeleteProgram(_water_shader);
}

void	Renderer::init(std::vector<Cell>& cells, int size) {
	_size = size;

	initMatrices();

	glEnable(GL_DEPTH_TEST);							// Enable 3D rendering
	glEnable(GL_MULTISAMPLE);							// MSAA (MultiSample Anti-Aliasing)
	glEnable(GL_BLEND);									// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// set blending for transparency

	initGround(cells);
	initWater(cells);
}

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

	// Define vertex attributes
		// void glVertexAttribPointer(
		//     GLuint index,          // Attribute index in the shader (layout location)
		//     GLint size,            // Number of components per vertex attribute
		//     GLenum type,           // Data type of each component
		//     GLboolean normalized,  // Whether to normalize the data
		//     GLsizei stride,        // Byte offset between consecutive attributes
		//     const void *pointer    // Byte offset to the first component of the attribute in the VBO
		// );
void	Renderer::initGround(std::vector<Cell>& cells) {
	glGenVertexArrays(1, &_groundVAO);	// Create  VAO
	glGenBuffers(1, &_groundVBO);		// generate VBO

	// Set VBO attributes
	glBindVertexArray(_groundVAO);		// Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, _groundVBO);	// Bind VBO on VAO
		// Position X Y Z (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		// Color R G B (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
		// Normal X Y Z (Location 2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Send data to the VBO
	std::vector<float> _ground_vertices = createGroundVertices(cells);
	glBufferData(GL_ARRAY_BUFFER, _ground_vertices.size() * sizeof(float), _ground_vertices.data(), GL_STATIC_DRAW);
	
	// Init Shader
	_ground_shader = createShaderProgram(GROUND_VERTEX_SHADER, GROUND_FRAGMENT_SHADER);
	initializeShader(_ground_shader);
}

void	Renderer::initWater(std::vector<Cell>& cells) {
	glGenVertexArrays(1, &_waterVAO);	// Create  VAO
	glBindVertexArray(_waterVAO);		// Bind VAO

	// Set StaticVBO attributes
	glGenBuffers(1, &_waterStaticVBO);		// generate VBO
	glBindBuffer(GL_ARRAY_BUFFER, _waterStaticVBO);	// Bind VBO
		// Position X Y (location 0)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		// Color R G B (location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Send data to the VBO
	std::vector<float> waterStaticVertices = createWaterStaticVertices();
	glBufferData(GL_ARRAY_BUFFER, waterStaticVertices.size() * sizeof(float), waterStaticVertices.data(), GL_STATIC_DRAW);

	// Set DynamicVBO attributes
	glGenBuffers(1, &_waterDynamicVBO);		// generate VBO
	glBindBuffer(GL_ARRAY_BUFFER, _waterDynamicVBO);	// Bind VBO
		// Position Z (location 2)
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
		// Depth (location 3)
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(1 * sizeof(float)));
	glEnableVertexAttribArray(3);
		// Normal X Y Z (location 4)
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// Send data to the VBO
	std::vector<float> waterDynamicVertices = createWaterDynamicVertices(cells);
	glBufferData(GL_ARRAY_BUFFER, waterDynamicVertices.size() * sizeof(float), waterDynamicVertices.data(), GL_DYNAMIC_DRAW);

	// Init Shader
	_water_shader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
	initializeShader(_water_shader);
}

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

glm::vec3 Renderer::calculateNormal(float up, float down, float left, float right) {
	float nx = left - right;
	float ny = down - up;
	float nz = 2.0f;

	glm::vec3 normal(nx, ny, nz);
	return glm::normalize(normal);
}

std::vector<float>	Renderer::createGroundVertices(std::vector<Cell>& cells) {
	std::vector<float>	vertices;
	glm::vec3 color1 = glm::vec3(0.0f, 0.7f, 0.0f);	// First triangle color
	glm::vec3 color2 = glm::vec3(0.0f, 0.7f, 0.0f);	// Second triangle color

	vertices.reserve((_size - 1) * (_size - 1) * 6 * 9); // pre-allocate to gain LOT of time

	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			// set heights[y][x] in a matrix
			std::array<std::array<float, 4>, 4> height = getHeightMatrix(cells, x, y, &Cell::getGroundLevel);

			// Calculate normals with the neighbors heights (up, down, left, right)
			const glm::vec3	normal[4] = {
				calculateNormal(height[2][1], height[0][1], height[1][0], height[1][2]), // SW
				calculateNormal(height[2][2], height[0][2], height[1][1], height[1][3]), // SE
				calculateNormal(height[3][2], height[1][2], height[2][1], height[2][3]), // NE
				calculateNormal(height[3][1], height[1][1], height[2][0], height[2][2]), // NW
			};

			// first triangle : SW -> SE -> NE
			pushVertex({x, y, height[1][1]}, vertices);
			pushVertex(color1, vertices);
			pushVertex(normal[0], vertices);
			pushVertex({x+1, y, height[1][2]}, vertices);
			pushVertex(color1, vertices);
			pushVertex(normal[1], vertices);
			pushVertex({x+1, y+1, height[2][2]}, vertices);
			pushVertex(color1, vertices);
			pushVertex(normal[2], vertices);

			// second triangle : NE -> NW -> SW
			pushVertex({x+1, y+1, height[2][2]}, vertices);
			pushVertex(color2, vertices);
			pushVertex(normal[2], vertices);
			pushVertex({x, y+1, height[2][1]}, vertices);
			pushVertex(color2, vertices);
			pushVertex(normal[3], vertices);
			pushVertex({x, y, height[1][1]}, vertices);
			pushVertex(color2, vertices);
			pushVertex(normal[0], vertices);
		}
	}
	return vertices;
}

// THE CURSED FUNCTIONS OF HEIGHTS AND NORMALS
//                         
//  3   y+2  0---X---X---0 
//           |   |   |   | 
//  2   y+1  X---NW--NE--X 
//           |   |   |   | 
//  1    y   X---SW--SE--X 
//           |   |   |   | 
//  0   y-1  0---X---X---0 
//          x-1  x  x+1 x+2
//                         
//           0   1   2   3 
std::array<std::array<float, 4>, 4> Renderer::getHeightMatrix(
	std::vector<Cell>& cells, int x, int y, float (Cell::*f)()) {
	// Define a 4x4 array initialized to zeros
	std::array<std::array<float, 4>, 4> matrix = {};

	// Fill the matrix [y][x]
	matrix[0][1] = (cells[index(x + 0, std::max(y - 1, 0))].*f)();
	matrix[0][2] = (cells[index(x + 1, std::max(y - 1, 0))].*f)();
	matrix[1][0] = (cells[index(std::max(x - 1, 0), y + 0)].*f)();
	matrix[1][1] = (cells[index(x + 0, y + 0)].*f)();
	matrix[1][2] = (cells[index(x + 1, y + 0)].*f)();
	matrix[1][3] = (cells[index(std::min(x + 2, _size - 1), y + 0)].*f)();
	matrix[2][0] = (cells[index(std::max(x - 1, 0), y + 1)].*f)();
	matrix[2][1] = (cells[index(x + 0, y + 1)].*f)();
	matrix[2][2] = (cells[index(x + 1, y + 1)].*f)();
	matrix[2][3] = (cells[index(std::min(x + 2, _size - 1), y + 1)].*f)();
	matrix[3][1] = (cells[index(x + 0, std::min(y + 2, _size - 1))].*f)();
	matrix[3][2] = (cells[index(x + 1, std::min(y + 2, _size - 1))].*f)();

	return matrix;
}

std::vector<float>	Renderer::createWaterDynamicVertices(std::vector<Cell>& cells) {
	std::vector<float>	vertices;

	vertices.reserve((_size - 1) * (_size - 1) * 6 * 5);

	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			// set heights[y][x] in a matrix
			std::array<std::array<float, 4>, 4> height = getHeightMatrix(cells, x, y, &Cell::getWaterVertexHeight);

			const glm::vec3	normal[4] = {
				calculateNormal(height[2][1], height[0][1], height[1][0], height[1][2]), // SW
				calculateNormal(height[2][2], height[0][2], height[1][1], height[1][3]), // SE
				calculateNormal(height[3][2], height[1][2], height[2][1], height[2][3]), // NE
				calculateNormal(height[3][1], height[1][1], height[2][0], height[2][2]), // NW
			};

			const float	depth[4] = {
					cells[index(x, y)].getWaterLevel(),
					cells[index(x + 1, y)].getWaterLevel(),
					cells[index(x + 1, y + 1)].getWaterLevel(),
					cells[index(x, y + 1)].getWaterLevel(),
			};

			// first triangle : SW -> SE -> NE
			vertices.push_back(height[1][1]);
			vertices.push_back(depth[0]);
			pushVertex(normal[0], vertices);
			vertices.push_back(height[1][2]);
			vertices.push_back(depth[1]);
			pushVertex(normal[1], vertices);
			vertices.push_back(height[2][2]);
			vertices.push_back(depth[2]);
			pushVertex(normal[2], vertices);

			// second triangle : NE -> NW -> SW
			vertices.push_back(height[2][2]);
			vertices.push_back(depth[2]);
			pushVertex(normal[2], vertices);
			vertices.push_back(height[2][1]);
			vertices.push_back(depth[3]);
			pushVertex(normal[3], vertices);
			vertices.push_back(height[1][1]);
			vertices.push_back(depth[0]);
			pushVertex(normal[0], vertices);
		}
	}
	return vertices;
}

std::vector<float>	Renderer::createWaterStaticVertices() {
	std::vector<float>	vertices;
	glm::vec3 color1 = glm::normalize(glm::vec3(34.0, 197.0, 255.0));
	glm::vec3 color2 = glm::normalize(glm::vec3(34.0, 197.0, 255.0));

	vertices.reserve((_size - 1) * (_size - 1) * 6 * 5);

	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			const glm::vec2 pos[4] = {
				{static_cast<float>(x), static_cast<float>(y)},
				{static_cast<float>(x + 1), static_cast<float>(y)},
				{static_cast<float>(x + 1), static_cast<float>(y + 1)},
				{static_cast<float>(x), static_cast<float>(y + 1)}
			};

			// first triangle : SW -> SE -> NE
			pushVertex(pos[0], vertices);
			pushVertex(color1, vertices);
			pushVertex(pos[1], vertices);
			pushVertex(color1, vertices);
			pushVertex(pos[2], vertices);
			pushVertex(color1, vertices);

			// second triangle : NE -> NW -> SW
			pushVertex(pos[2], vertices);
			pushVertex(color2, vertices);
			pushVertex(pos[3], vertices);
			pushVertex(color2, vertices);
			pushVertex(pos[0], vertices);
			pushVertex(color2, vertices);
		}
	}
	return vertices;
}

void	Renderer::pushVertex(glm::vec2 vertex, std::vector<float>& dest) {
	dest.push_back(vertex.x);
	dest.push_back(vertex.y);
}

void	Renderer::pushVertex(glm::vec3 vertex, std::vector<float>& dest) {
	dest.push_back(vertex.x);
	dest.push_back(vertex.y);
	dest.push_back(vertex.z);
}

int	Renderer::index(int x, int y) {
	return x + y * _size;
}

void	Renderer::initializeShader(GLint shader) {
	glUseProgram(shader);

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

	glUseProgram(0);
}

void	Renderer::initMatrices() {
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
}

void	Renderer::render(WaterSurface& surface, Camera& camera) {
	int	vertexCount = (_size - 1) * (_size - 1) * 6;

	setupCamera(camera);

	// Draw Ground
	glUseProgram(_ground_shader);
	glBindVertexArray(_groundVAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	// Draw Water
	glUseProgram(_water_shader);
	glBindVertexArray(_waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _waterDynamicVBO);
	std::vector<float> waterDynamicVertices = createWaterDynamicVertices(surface.getCells());
	glBufferData(GL_ARRAY_BUFFER, waterDynamicVertices.size() * sizeof(float), waterDynamicVertices.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	// Check OpenGL errors
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
		// Handle the error
	}
}


// std::vector<float> Renderer::createWaterDynamicVertices(std::vector<Cell>& cells) {
// 	const int numThreads = std::thread::hardware_concurrency(); // Get the number of threads available
// 	const int rowsPerThread = _size / numThreads;               // Divide grid
// 	std::vector<std::vector<float>> threadResults(numThreads);  // Temporary storage for thread results
// 	std::mutex mutex;

// 	// Worker function for each thread
// 	auto worker = [&](int startRow, int endRow, int threadIndex) {
// 		std::vector<float> localVertices;

// 		for (int y = startRow; y < endRow; ++y) {
// 			for (int x = 0; x < _size - 1; ++x) {
// 			// set heights[y][x] in a matrix
// 			std::array<std::array<float, 4>, 4> height = getHeightMatrix(cells, x, y, &Cell::getWaterVertexHeight);

// 			// const float	height[4][4] = {
// 			// 	{	0,
// 			// 		cells[index(x + 0, std::max(y - 1, 0))].getWaterVertexHeight(),
// 			// 		cells[index(x + 1, std::max(y - 1, 0))].getWaterVertexHeight(),
// 			// 		0,
// 			// 	},
// 			// 	{	cells[index(std::max(x - 1, 0), y + 0)].getWaterVertexHeight(),
// 			// 		cells[index(x + 0, y + 0)].getWaterVertexHeight(),
// 			// 		cells[index(x + 1, y + 0)].getWaterVertexHeight(),
// 			// 		cells[index(std::min(x + 2, _size - 1), y + 0)].getWaterVertexHeight(),
// 			// 	},
// 			// 	{	cells[index(std::max(x - 1, 0), y + 1)].getWaterVertexHeight(),
// 			// 		cells[index(x + 0, y + 1)].getWaterVertexHeight(),
// 			// 		cells[index(x + 1, y + 1)].getWaterVertexHeight(),
// 			// 		cells[index(std::min(x + 2, _size - 1), y + 1)].getWaterVertexHeight(),
// 			// 	},
// 			// 	{	0,
// 			// 		cells[index(x + 0, std::min(y + 2, _size - 1))].getWaterVertexHeight(),
// 			// 		cells[index(x + 1, std::min(y + 2, _size - 1))].getWaterVertexHeight(),
// 			// 		0,
// 			// 	}
// 			// };

// 			const glm::vec3	normal[4] = {
// 				calculateNormal(height[2][1], height[0][1], height[1][0], height[1][2]), // SW
// 				calculateNormal(height[2][2], height[0][2], height[1][1], height[1][3]), // SE
// 				calculateNormal(height[3][2], height[1][2], height[2][1], height[2][3]), // NE
// 				calculateNormal(height[3][1], height[1][1], height[2][0], height[2][2]), // NW
// 			};

// 			const float	depth[4] = {
// 					cells[index(x, y)].getWaterLevel(),
// 					cells[index(x + 1, y)].getWaterLevel(),
// 					cells[index(x + 1, y + 1)].getWaterLevel(),
// 					cells[index(x, y + 1)].getWaterLevel(),
// 			};

// 			// first triangle : SW -> SE -> NE
// 			localVertices.push_back(height[1][1]);
// 			localVertices.push_back(depth[0]);
// 			pushVertex(normal[0], localVertices);
// 			localVertices.push_back(height[1][2]);
// 			localVertices.push_back(depth[1]);
// 			pushVertex(normal[1], localVertices);
// 			localVertices.push_back(height[2][2]);
// 			localVertices.push_back(depth[2]);
// 			pushVertex(normal[2], localVertices);
// 			// second triangle : NE -> NW -> SW
// 			localVertices.push_back(height[2][2]);
// 			localVertices.push_back(depth[2]);
// 			pushVertex(normal[2], localVertices);
// 			localVertices.push_back(height[2][1]);
// 			localVertices.push_back(depth[3]);
// 			pushVertex(normal[3], localVertices);
// 			localVertices.push_back(height[1][1]);
// 			localVertices.push_back(depth[0]);
// 			pushVertex(normal[0], localVertices);
// 		}
// 	}

// 		// Store local results in threadResults
// 		std::lock_guard<std::mutex> lock(mutex); // Lock when constructed, unlock when destructed
// 		threadResults[threadIndex] = std::move(localVertices);
// 	};

// 	// Create threads
// 	std::vector<std::thread> threads;
// 	for (int t = 0; t < numThreads; ++t) {
// 		int startRow = t * rowsPerThread;
// 		int endRow = (t == numThreads - 1) ? _size - 1 : startRow + rowsPerThread;
// 		threads.emplace_back(worker, startRow, endRow, t);
// 	}

// 	// Wait for all threads to finish
// 	for (auto& thread : threads) {
// 		thread.join();
// 	}

// 	// Merge all thread results
// 	std::vector<float> vertices;
// 	for (auto& result : threadResults) {
// 		vertices.insert(vertices.end(), result.begin(), result.end());
// 	}

// 	return vertices;
// }
