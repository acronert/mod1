#ifndef RENDERER_CLASS_HPP
# define RENDERER_CLASS_HPP

#include "glew.h"
// #include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>		// For perspective projection
#include <GLFW/glfw3.h>	// For window and context management
#include <math.h>

// FOR THE SHADER CREATE METHOD
#include <sstream>
#include <fstream>

#include "WaterSurface.class.hpp"
#include "Camera.class.hpp"

#include <iostream>
#include <vector>
#include <array>

#include <thread>
#include <mutex>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" // for value_ptr

# define DEG_TO_RAD			M_PI / 180.0f
// # define WATER_TEXTURE_PATH	"texture/water.jpg"

# define WATER_VERTEX_SHADER	"shader/water_vertex_shader.glsl"
# define WATER_FRAGMENT_SHADER	"shader/water_fragment_shader.glsl"
# define GROUND_VERTEX_SHADER	"shader/ground_vertex_shader.glsl"
# define GROUND_FRAGMENT_SHADER	"shader/ground_fragment_shader.glsl"

struct s_vec3 {
	float x;
	float y;
	float z;
};

struct s_vec2 {
	float x;
	float y;
};

class Renderer
{
	private:
		int					_size;

		bool				_initialized;

		GLuint				_waterVAO;
		GLuint				_waterStaticVBO;
		GLuint				_waterDynamicVBO;
		GLint				_water_shader;

		GLuint				_groundVAO;
		GLuint				_groundVBO;
		GLint				_ground_shader;

		// Matrices
		glm::mat4			_projection;
		glm::mat4			_view;
		glm::mat4			_model;


	public:
		Renderer();
		~Renderer();

		void	render(WaterSurface& surface, Camera& camera);
		void	setupCamera(Camera& camera);
		// std::vector<float>	createWaterVertices(std::vector<Cell>& cells);
		std::vector<float>	createWaterStaticVertices();
		std::vector<float>	createGroundVertices(std::vector<Cell>& cells);
		std::vector<float>	createWaterDynamicVertices(std::vector<Cell>& cells);

		// GLuint	loadTexture(const char* filename);

		void	init(std::vector<Cell>& cells, int size);
		void	initMatrices();
		void	initGround(std::vector<Cell>& cells);
		void	initWater(std::vector<Cell>& cells);

		GLint	createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
		GLint	loadShader(const char* filepath, GLenum shaderType);
		void	initializeShader(GLint shader);
		void	pushVertex(glm::vec3 vertex, std::vector<float>& dest);
		void	pushVertex(glm::vec2 vertex, std::vector<float>& dest);

		std::array<std::array<float, 4>, 4> getHeightMatrix(std::vector<Cell>& cells, int x, int y, float (Cell::*f)());

		// glm::vec3	calculateNormal(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);
		glm::vec3	calculateNormal(float up, float down, float left, float right);


		int		index(int x, int y);
};

#endif