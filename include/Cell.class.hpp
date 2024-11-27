#ifndef CELL_CLASS_HPP
# define CELL_CLASS_HPP

# include "mod1.hpp"
# include <iostream>
# include "glm/glm.hpp"

class Cell
{
	private:
		// WaterLevel
		float	_w;
		float*	_wN;	// WaterLevel N (pointer to neighbor cell N)
		float*	_wE;	// WaterLevel E (pointer to neighbor cell E)
		float*	_wS;	// WaterLevel S (pointer to neighbor cell S)
		float*	_wW;	// WaterLevel W (pointer to neighbor cell W)

		// GroundLevel
		float	_g;
		float*	_gN;	// GroundLevel N (pointer to neighbor cell N)
		float*	_gE;	// GroundLevel E (pointer to neighbor cell E)
		float*	_gS;	// GroundLevel S (pointer to neighbor cell S)
		float*	_gW;	// GroundLevel W (pointer to neighbor cell W)

		// Velocity
		float	_totalVelocity; // sum of the velocities
		float	_vN;	// velocity N
		float	_vE;	// velocity W
		float*	_vS;	// velocity S (pointer to neighbor cell vN)
		float*	_vW;	// velocity E (pointer to neighbor cell vW)

		glm::vec3	_normal;
		glm::vec3	_groundNormal;

	public:
		Cell();
		Cell(float w, float g, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell);
		~Cell();
		Cell(const Cell& other);
		Cell& operator=(const Cell& other);

		float	acceleration(float other_w, float other_g);
		void	updateVelocity();
		void	calculateTotalVelocity();
		void	updateWaterLevel();
		void	resolveUnderflow();

		void	updateNormal();
		glm::vec3	getNormal() const;

		void	updateGroundNormal();
		glm::vec3	getGroundNormal() const;


		void	setWaterLevel(float w);
		void	setGroundLevel(float g);

		void	resetWater();
		void	addWater(float intensity);
		void	addVelocity(float vN, float vE, float vS, float vW);

		float	getTotalLevel();
		float	getWaterLevel();
		float	getGroundLevel();
		
		float	getVelocityN();
		float	getVelocityE();
		float	getVelocityS();
		float	getVelocityW();

		float	getWaterVertexHeight(); // to hangle wall hugging cases
};

#endif
