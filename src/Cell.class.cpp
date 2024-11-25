#include "Cell.class.hpp"

Cell::Cell() : _w(0.0f), _wN(nullptr), _wE(nullptr), _wS(nullptr), _wW(nullptr),
	_g(0.0f), _gN(nullptr), _gE(nullptr), _gS(nullptr), _gW(nullptr),
	_totalVelocity(0.0f), _vN(0.0f), _vE(0.0f), _vS(nullptr), _vW(nullptr), _normal(0.f)
{}

Cell::Cell(float w, float g, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell)
	: _w(w), _wN(nullptr), _wE(nullptr), _wS(nullptr), _wW(nullptr),
	_g(g), _gN(nullptr), _gE(nullptr), _gS(nullptr), _gW(nullptr),
	_totalVelocity(0.0f) ,_vN(0.0f), _vE(0.0f), _vS(nullptr), _vW(nullptr), _normal(0.f)
{
	if (Ncell != nullptr) {
		_wN = &(Ncell->_w);
		_gN = &(Ncell->_g);
	}
	if (Ecell != nullptr){
		_wE = &(Ecell->_w);
		_gE = &(Ecell->_g);
	}
	if (Scell != nullptr){
		_wS = &(Scell->_w);
		_gS = &(Scell->_g);
		_vS = &(Scell->_vN);
	}
	if (Wcell != nullptr) {
		_wW = &(Wcell->_w);
		_gW = &(Wcell->_g);
		_vW = &(Wcell->_vE);
	}
}

Cell::~Cell() {}

Cell::Cell(const Cell& other)
	: _w(other._w),	_g(other._g), _totalVelocity(other._totalVelocity),
	_vN(other._vN), _vE(other._vE), _normal(other._normal)
{
	_wN = other._wN != nullptr ? other._wN : nullptr;
	_wE = other._wE != nullptr ? other._wE : nullptr;
	_wS = other._wS != nullptr ? other._wS : nullptr;
	_wW = other._wW != nullptr ? other._wW : nullptr;

	_gN = other._gN != nullptr ? other._gN : nullptr;
	_gE = other._gE != nullptr ? other._gE : nullptr;
	_gS = other._gS != nullptr ? other._gS : nullptr;
	_gW = other._gW != nullptr ? other._gW : nullptr;

	_vS = other._vS != nullptr ? other._vS : nullptr;
	_vW = other._vW != nullptr ? other._vW : nullptr;
}

Cell& Cell::operator=(const Cell& other) {
	if (this != &other) {
		_w = other._w;
		_g = other._g;
		_totalVelocity = other._totalVelocity;
		_vN = other._vN;
		_vE = other._vE;

		_wN = other._wN != nullptr ? other._wN : nullptr;
		_wE = other._wE != nullptr ? other._wE : nullptr;
		_wS = other._wS != nullptr ? other._wS : nullptr;
		_wW = other._wW != nullptr ? other._wW : nullptr;

		_gN = other._gN != nullptr ? other._gN : nullptr;
		_gE = other._gE != nullptr ? other._gE : nullptr;
		_gS = other._gS != nullptr ? other._gS : nullptr;
		_gW = other._gW != nullptr ? other._gW : nullptr;

		_vS = other._vS != nullptr ? other._vS : nullptr;
		_vW = other._vW != nullptr ? other._vW : nullptr;
		_normal = other._normal;
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

// Acceleration = PROPAGATION * (neighbor.h - target.h)
	// Compare tallest and max(tallest.g, smallest.g + smallest.w)
float	Cell::acceleration(float other_w, float other_g) {
	float	other_height;
	float	height;

	if (other_w + other_g > _w + _g) {
		other_height = other_w + other_g;
		height = std::max(other_g, _w + _g);
	} else {
		height = _w + _g;
		other_height = std::max(_g, other_w + other_g);
	}

	return DELTA_TIME * PROPAGATION * (other_height - height);
}

// Velocity = (DAMPENING * target.v) + (DELTA_TIME * target.a)
void	Cell::updateVelocity() {
	if (_wN != nullptr)
		_vN = (DAMPENING * _vN) + acceleration(*_wN, *_gN);
	else
		_vN = 0;

	if (_wE != nullptr)
		_vE = (DAMPENING * _vE) + acceleration(*_wE, *_gE);
	else
		_vE = 0;
	
	calculateTotalVelocity();
}

// WaterLevel = DELTA_TIME * (vN + vE + vS + vW)
	// _vS and _vW are inverted to get the right direction
void	Cell::calculateTotalVelocity() {
	_totalVelocity = _vN + _vE;
	_totalVelocity += _vS ? -*_vS : 0;
	_totalVelocity += _vW ? -*_vW : 0;
}

void	Cell::resolveUnderflow() {
	if (_w >= 0)
		return;

	float	v_underflow = (_w + (DELTA_TIME * _totalVelocity)) / DELTA_TIME;
	int		positive_neighbors = 0;

	positive_neighbors += _vN < 0;
	positive_neighbors += _vE < 0;
	positive_neighbors += _vS != nullptr ? (-*_vS < 0): 0;
	positive_neighbors += _vW != nullptr ? (-*_vW < 0): 0;

	if (_vN < 0) {
		_vN += v_underflow / positive_neighbors;
		*_wN += _w / positive_neighbors;
	}
	if (_vE < 0) {
		_vE += v_underflow / positive_neighbors;
		*_wE += _w / positive_neighbors;
	}
	if (_vS != nullptr && -*_vS < 0) {
		*_vS += v_underflow / positive_neighbors;
		*_wS += _w / positive_neighbors;
	}
	if (_vW != nullptr && -*_vW < 0) {
		*_vW += v_underflow / positive_neighbors;
		*_wW += _w / positive_neighbors;
	}
	
	_vN = 0;
	_vE = 0;
	if (_vS != nullptr)
		*_vS = 0;
	if (_vW != nullptr)
		*_vW = 0;
	_w = 0;
}

float	Cell::getWaterVertexHeight() {
	if (_w > 0.001)
		return _g + _w;

	// count neighbors that have water and return their _g + _w mean
	float	vertexHeight = 0;
	int		neighbors = 0;

	if (_wN && *_wN > 0.001) {
		neighbors++;
		vertexHeight += *_wN + *_gN;
	}
	if (_wE && *_wE > 0.001) {
		neighbors++;
		vertexHeight += *_wE + *_gE;
	}
	if (_wS && *_wS > 0.001) {
		neighbors++;
		vertexHeight += *_wS + *_gS;
	}
	if (_wW && *_wW > 0.001) {
		neighbors++;
		vertexHeight += *_wW + *_gW;
	}
	vertexHeight = neighbors ? vertexHeight / neighbors : 0;
	return vertexHeight;
}

void	Cell::resetWater() {
	_w = 0.0f;
	_vN = 0.0f;
	_vE = 0.0f;
}

void	Cell::addWater(float intensity) {
	_w += intensity;
	if (_w < 0)
		_w = 0;
}

void	Cell::addVelocity(float vN, float vE, float vS, float vW) {
	_vN += vN;
	_vE += vE;
	if (_vS)
		*_vS += vS;
	if (_vW)
		*_vW += vW;
}

void	Cell::updateWaterLevel() { _w += DELTA_TIME * _totalVelocity; }

float	Cell::getTotalLevel() { return _w + _g; }
float	Cell::getWaterLevel() { return _w; }
float	Cell::getGroundLevel() { return _g; }

void	Cell::setWaterLevel(float w){ _w = w; }
void	Cell::setGroundLevel(float g){ _g = g; }

float	Cell::getVelocityN() { return _vN; }
float	Cell::getVelocityE() { return _vE; }
float	Cell::getVelocityS() { return _vS ? *_vS : 0; }
float	Cell::getVelocityW() { return _vW ? *_vW : 0; }
glm::vec3	Cell::getNormal() const { return _normal; }

void	Cell::updateNormal(void) {
	float nx = (_wW ? *_wW : _w) - (_wE ? *_wE : _w);
	float ny = (_wS ? *_wS : _w) - (_wN ? *_wN : _w);
	float nz = 2.0f;

	glm::vec3 normal(nx, ny, nz);
	_normal = glm::normalize(normal);
}
