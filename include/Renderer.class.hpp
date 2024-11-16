#ifndef RENDERER_CLASS_HPP
# define RENDERER_CLASS_HPP

#include <GL/glew.h>
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

class Renderer
{
	private:
		int					_size;

		GLuint				_waterVAO;
		GLuint				_groundVAO;

		GLuint				_waterVBO;
		GLuint				_groundVBO;

		GLint				_water_shader;
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
		std::vector<float>	createWaterVertices(std::vector<Cell>& cells);
		std::vector<float>	createGroundVertices(std::vector<Cell>& cells);
		std::vector<float>	updateWaterHeightVertices(std::vector<Cell>& cells);

		// GLuint	loadTexture(const char* filename);

		void	init(std::vector<Cell>& cells, int size);
		void	initMatrices();
		void	initGroundVBO();
		void	initWaterVBO();

		GLint	createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
		GLint	loadShader(const char* filepath, GLenum shaderType);
		void	pushQuadVertex(s_vec3 quad, s_vec3 color, std::vector<float>& vertices);
		void	initializeShader(GLint shader);



		int		index(int x, int y);
};

#endif