#include "Renderer.class.hpp"

Renderer::Renderer() {
	// _waterTexture = loadTexture(WATER_TEXTURE_PATH);

	// _waterShader = createShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER);
}

Renderer::~Renderer() {}


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
				{static_cast<float>(x), static_cast<float>(y), 
				cells[index(x, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y),
				cells[index(x + 1, y)].getGroundLevel()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),
				cells[index(x + 1, y + 1)].getGroundLevel()},
				{static_cast<float>(x), static_cast<float>(y + 1),
				cells[index(x, y + 1)].getGroundLevel()},
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
	glEnable(GL_BLEND);			// Blend for transparency

	glBegin(GL_TRIANGLES);

	for (int y = 0; y < _sizeY - 1; ++y) {
		for (int x = 0; x < _sizeX - 1; ++x) {
			// Generate water vertices
			const float vertices[4][3] = {
				{static_cast<float>(x), static_cast<float>(y), 
				cells[index(x, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y),
				cells[index(x + 1, y)].getWaterVertexHeight()},
				{static_cast<float>(x + 1), static_cast<float>(y + 1),
				cells[index(x + 1, y + 1)].getWaterVertexHeight()},
				{static_cast<float>(x), static_cast<float>(y + 1),
				cells[index(x, y + 1)].getWaterVertexHeight()},
			};
			const float waterLevel[4] = {
				cells[index(x, y)].getWaterLevel(),
				cells[index(x + 1, y)].getWaterLevel(),
				cells[index(x + 1, y + 1)].getWaterLevel(),
				cells[index(x, y + 1)].getWaterLevel()
			};

			// first triangle
				glColor4f(0.0, 0.0, 1.0, 0.7 * std::min(1.0f, waterLevel[0])); // RGBA
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
				glColor4f(0.0, 0.0, 1.0, 0.7 * std::min(1.0f, waterLevel[1])); // RGBA
				glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
				glColor4f(0.0, 0.0, 1.0, 0.7 * std::min(1.0f, waterLevel[2])); // RGBA
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

			// second triangle
				glColor4f(0.0, 0.0, 0.7, 0.7 * std::min(1.0f, waterLevel[2])); // RGBA
				glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
				glColor4f(0.0, 0.0, 0.7, 0.7 * std::min(1.0f, waterLevel[3])); // RGBA
				glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
				glColor4f(0.0, 0.0, 0.7, 0.7 * std::min(1.0f, waterLevel[0])); // RGBA
				glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
		}
	}

	glEnd();
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