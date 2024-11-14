#ifndef RENDERER_CLASS_HPP
# define RENDERER_CLASS_HPP

#include <GL/gl.h>
#include <GL/glu.h>		// For perspective projection
// #include <GL/glew.h>
#include <GLFW/glfw3.h>	// For window and context management
#include <math.h>

// FOR THE SHADER CREATE METHOD
#include <sstream>
#include <fstream>

#include "WaterSurface.class.hpp"
#include "Camera.class.hpp"

// #include <SOIL/SOIL.h>	// for loading textures
#include <iostream>
#include <vector>

# define DEG_TO_RAD			M_PI / 180.0f
// # define WATER_TEXTURE_PATH	"texture/water.jpg"

# define WATER_VERTEX_SHADER	"shader/vertex_shader.glsl"
# define WATER_FRAGMENT_SHADER	"shader/fragment_shader.glsl"

struct Vertex {
	float x;
	float y;
	float z;
};

class Renderer
{
	private:
		std::vector<Vertex>	_waterVertices;
		std::vector<Vertex>	_groundVertices;
		int					_sizeX;
		int					_sizeY;

		GLuint				_waterShader;
		// GLuint				_waterTexture;

	public:
		Renderer();
		~Renderer();

		void	render(WaterSurface& surface, Camera& camera);
		void	setupCamera(Camera& camera);
		void	generateVertices(std::vector<Cell>& cells);
		void	drawWaterVertices();
		void	drawGroundVertices();

		// GLuint	loadTexture(const char* filename);

		GLuint	createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
		GLuint	loadShader(const char* filepath, GLenum shaderType);


		int		index(int x, int y);
};

#endif