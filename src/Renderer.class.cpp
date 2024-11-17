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

Renderer::~Renderer() {
	// Cleanup
		glDeleteVertexArrays(1, &_groundVAO);
		glDeleteVertexArrays(1, &_waterVAO);
		glDeleteBuffers(1, &_groundVBO);
		glDeleteBuffers(1, &_waterStaticVBO);
		glDeleteBuffers(1, &_waterDynamicVBO);
		glDeleteProgram(_ground_shader);
		glDeleteProgram(_water_shader);
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

glm::vec3	Renderer::calculateNormal(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2) {
	// Calculate two vectors from the triangle's vertices
	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;

	// Compute the normal using the cross product
	glm::vec3 normal = cross(edge1, edge2);
	return normalize(normal);
}

// height and normal
std::vector<float>	Renderer::createWaterDynamicVertices(std::vector<Cell>& cells) {
	std::vector<float>	vertices;

	vertices.reserve((_size - 1) * (_size - 1) * 6 * 4);

	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			// Find quads	(SW, SE, NE, NW)
			const float height[4] = {
				{cells[index(x, y)].getWaterVertexHeight()},
				{cells[index(x + 1, y)].getWaterVertexHeight()},
				{cells[index(x + 1, y + 1)].getWaterVertexHeight()},
				{cells[index(x, y + 1)].getWaterVertexHeight()},
			};
			glm::vec3 position[4] = {
				{static_cast<float>(x), static_cast<float>(y), height[0]},
				{static_cast<float>(x + 1), static_cast<float>(y), height[1]},
				{static_cast<float>(x + 1), static_cast<float>(y + 1), height[2]},
				{static_cast<float>(x), static_cast<float>(y + 1), height[3]},
			};

			// Calculate normal (cross product of 2 edges)
			glm::vec3 normal1 = calculateNormal(position[0], position[1], position[2]);
			glm::vec3 normal2 = calculateNormal(position[2], position[3], position[0]);

		// std::cout << "normal1 [" << x << "][" << y << "] = " << std::endl;
		// std::cout << "   " << normal1.x << std::endl;
		// std::cout << "   " << normal1.y << std::endl;
		// std::cout << "   " << normal1.z << std::endl;


		// std::cout << "normal2 [" << x << "][" << y << "] = " << std::endl;
		// std::cout << "   " << normal2.x << std::endl;
		// std::cout << "   " << normal2.y << std::endl;
		// std::cout << "   " << normal2.z << std::endl;

			// first triangle : 0 -> 1 -> 2
			vertices.push_back(height[0]);
			vertices.push_back(normal1.x);
			vertices.push_back(normal1.y);
			vertices.push_back(normal1.z);

			vertices.push_back(height[1]);
			vertices.push_back(normal1.x);
			vertices.push_back(normal1.y);
			vertices.push_back(normal1.z);

			vertices.push_back(height[2]);
			vertices.push_back(normal1.x);
			vertices.push_back(normal1.y);
			vertices.push_back(normal1.z);

			// second triangle : 2 -> 3 -> 0
			vertices.push_back(height[2]);
			vertices.push_back(normal2.x);
			vertices.push_back(normal2.y);
			vertices.push_back(normal2.z);

			vertices.push_back(height[3]);
			vertices.push_back(normal2.x);
			vertices.push_back(normal2.y);
			vertices.push_back(normal2.z);

			vertices.push_back(height[0]);
			vertices.push_back(normal2.x);
			vertices.push_back(normal2.y);
			vertices.push_back(normal2.z);
		}
	}
	return vertices;
}

void	Renderer::pushQuadVertex(s_vec3 pos, s_vec3 color, std::vector<float>& vertices) {
	vertices.push_back(pos.x);
	vertices.push_back(pos.y);
	vertices.push_back(pos.z);
	vertices.push_back(color.x);	// r
	vertices.push_back(color.y);	// g
	vertices.push_back(color.z);	// b
}

void	Renderer::pushQuadVertex(s_vec2 pos, s_vec3 color, std::vector<float>& vertices) {
	vertices.push_back(pos.x);
	vertices.push_back(pos.y);
	vertices.push_back(color.x);	// r
	vertices.push_back(color.y);	// g
	vertices.push_back(color.z);	// b
}

std::vector<float>	Renderer::createWaterStaticVertices() {

	std::vector<float>	vertices;

	// pre-allocate to gain time
	vertices.reserve((_size - 1) * (_size - 1) * 6 * 5);
	s_vec3 color1 = {0.0, 0.0, 1.0};
	s_vec3 color2 = {0.0, 0.0, 1.0};

	// FILL THE VERTICES ////////////
	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			// Find quads	(SW, SE, NE, NW)
			const s_vec2 quad[4] = {
				{static_cast<float>(x), static_cast<float>(y)},
				{static_cast<float>(x + 1), static_cast<float>(y)},
				{static_cast<float>(x + 1), static_cast<float>(y + 1)},
				{static_cast<float>(x), static_cast<float>(y + 1)}
			};

			// first triangle : 0 -> 1 -> 2
			pushQuadVertex(quad[0], color1, vertices);
			pushQuadVertex(quad[1], color1, vertices);
			pushQuadVertex(quad[2], color1, vertices);

			// second triangle : 2 -> 3 -> 0
			pushQuadVertex(quad[2], color2, vertices);
			pushQuadVertex(quad[3], color2, vertices);
			pushQuadVertex(quad[0], color2, vertices);
		}
	}
	return vertices;
}

// std::vector<float> Renderer::createWaterVertices(std::vector<Cell>& cells) {
// 	const int numThreads = std::thread::hardware_concurrency(); // Get the number of threads available
// 	const int rowsPerThread = _size / numThreads;               // Divide grid
// 	std::vector<std::vector<float>> threadResults(numThreads);  // Temporary storage for thread results
// 	std::mutex mutex;

// 	// Worker function for each thread
// 	auto worker = [&](int startRow, int endRow, int threadIndex) {
// 		std::vector<float> localVertices;

// 		for (int y = startRow; y < endRow; ++y) {
// 			for (int x = 0; x < _size - 1; ++x) {
// 				// Find quads (SW, SE, NE, NW)
// 				const s_vec3 quad[4] = {
// 					{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getWaterVertexHeight()},
// 					{static_cast<float>(x + 1), static_cast<float>(y), cells[index(x + 1, y)].getWaterVertexHeight()},
// 					{static_cast<float>(x + 1), static_cast<float>(y + 1), cells[index(x + 1, y + 1)].getWaterVertexHeight()},
// 					{static_cast<float>(x), static_cast<float>(y + 1), cells[index(x, y + 1)].getWaterVertexHeight()},
// 				};

// 				// Add vertices for two triangles
// 				s_vec3 color1 = {0.0, 0.0, 0.7}; // First triangle
// 				pushQuadVertex(quad[0], color1, localVertices);
// 				pushQuadVertex(quad[1], color1, localVertices);
// 				pushQuadVertex(quad[2], color1, localVertices);

// 				s_vec3 color2 = {0.0, 0.0, 0.5}; // Second triangle
// 				pushQuadVertex(quad[2], color2, localVertices);
// 				pushQuadVertex(quad[3], color2, localVertices);
// 				pushQuadVertex(quad[0], color2, localVertices);
// 			}
// 		}

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


std::vector<float>	Renderer::createGroundVertices(std::vector<Cell>& cells) {
	std::vector<float>	vertices; // x1, y1, height1, r1, g1, b1, x2, y2, height2, r2, g2, b2, ...

	// pre-allocate to gain time
	vertices.reserve((_size - 1) * (_size - 1) * 6 * 6);
	s_vec3 color1 = {0.0, 0.7, 0.0};
	s_vec3 color2 = {0.0, 0.5, 0.0};

	for (int y = 0; y < _size - 1; ++y) {
		for (int x = 0; x < _size - 1; ++x) {
			// Find quads	(SW, SE, NE, NW)
			const s_vec3 quad[4] = {
				{static_cast<float>(x), static_cast<float>(y), cells[index(x, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y),cells[index(x + 1, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),cells[index(x + 1, y + 1)].getGroundLevel()},
				{static_cast<float>(x), static_cast<float>(y + 1),cells[index(x, y + 1)].getGroundLevel()},
			};

			// first triangle : 0 -> 1 -> 2
			pushQuadVertex(quad[0], color1, vertices);
			pushQuadVertex(quad[1], color1, vertices);
			pushQuadVertex(quad[2], color1, vertices);

			// second triangle : 2 -> 3 -> 0
			pushQuadVertex(quad[2], color2, vertices);
			pushQuadVertex(quad[3], color2, vertices);
			pushQuadVertex(quad[0], color2, vertices);
		}
	}
	return vertices;
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

	glBindVertexArray(_groundVAO);		// Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, _groundVBO);	// Bind VBO on VAO

		// Define attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);	// Location 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	glEnableVertexAttribArray(1);	// Location 1

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


	// STATIC VBO //
	glGenBuffers(1, &_waterStaticVBO);		// generate VBO
	glBindBuffer(GL_ARRAY_BUFFER, _waterStaticVBO);	// Bind VBO
		// Position X Y (location 0)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		// Color R G B (location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
		// Send initial data to the VBO
	std::vector<float> waterStaticVertices = createWaterStaticVertices();
	glBufferData(GL_ARRAY_BUFFER, waterStaticVertices.size() * sizeof(float), waterStaticVertices.data(), GL_STATIC_DRAW);


	// DYNAMIC VBO //
	glGenBuffers(1, &_waterDynamicVBO);		// generate VBO
	glBindBuffer(GL_ARRAY_BUFFER, _waterDynamicVBO);	// Bind VBO
		// Position Z (location 2)
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
		// Normal X Y Z (location 3)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(1 * sizeof(float)));
	glEnableVertexAttribArray(3);

	std::vector<float> waterDynamicVertices = createWaterDynamicVertices(cells);
	glBufferData(GL_ARRAY_BUFFER, waterDynamicVertices.size() * sizeof(float), waterDynamicVertices.data(), GL_DYNAMIC_DRAW);


	// Init Shader
	_water_shader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
	initializeShader(_water_shader);

}

void	Renderer::init(std::vector<Cell>& cells, int size) {
	_size = size;

	// Initialize matrices
	initMatrices();

	// Render Settings
	glEnable(GL_DEPTH_TEST);							// Enable 3D rendering
	glEnable(GL_MULTISAMPLE);							// MSAA (MultiSample Anti-Aliasing)
	glEnable(GL_BLEND);									// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// set blending for transparency

	initGround(cells);
	initWater(cells);

	
}

void	Renderer::render(WaterSurface& surface, Camera& camera) {

	setupCamera(camera);

	int	vertexCount = (_size - 1) * (_size - 1) * 6;

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


		// Update the dynamic water VBO
	// glBindBuffer(GL_ARRAY_BUFFER, _waterStaticVBO);
	// std::vector<float> water_height = updateWaterHeightVertices(surface.getCells());
	// const size_t	stride = 6 * sizeof(float);
	// const size_t	offset = 2 * sizeof(float);
	// glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount * sizeof(float), water_height.data());

	// std::vector<float> water_vertices = createWaterVertices(surface.getCells());
	// glBufferData(GL_ARRAY_BUFFER, water_vertices.size() * sizeof(float), water_vertices.data(), GL_DYNAMIC_DRAW);
	// glDrawArrays(GL_TRIANGLES, 0, vertexCount);


	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
		// Handle the error
	}
}
