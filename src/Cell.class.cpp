#include "Cell.class.hpp"

Cell::Cell() : _w(0), _totalVelocity(0), _vN(0), _vE(0), _vS(nullptr), _vW(nullptr) {}

Cell::Cell(float w, float g, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell)
	: _w(w), _wN(nullptr), _wE(nullptr), _wS(nullptr), _wW(nullptr),
	_g(g), _gN(nullptr), _gE(nullptr), _gS(nullptr), _gW(nullptr),
	_totalVelocity(0.0f) ,_vN(0.0f), _vE(0.0f), _vS(nullptr), _vW(nullptr)
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
	_vN(other._vN), _vE(other._vE)
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
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

// Acceleration = PROPAGATION * (neighbor.h - target.h)
// Velocity = (DAMPENING * target.v) + (DELTA_TIME * target.a)
void	Cell::updateVelocity() {

	if (_wN != nullptr)
		_vN = (DAMPENING * _vN) + (DELTA_TIME * PROPAGATION * (*_wN - _w));
	else
		_vN = 0;

	if (_wE != nullptr)
		_vE = (DAMPENING * _vE) + (DELTA_TIME * PROPAGATION * (*_wE - _w));
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

	// std::cout << "UNDERFLOW" << std::endl;
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
	
	_totalVelocity -= v_underflow;
	_w = 0;
}

void	Cell::updateWaterLevel() { _w += DELTA_TIME * _totalVelocity; }

float	Cell::getWaterLevel() { return _w; }

float	Cell::getGroundLevel() { return _g; }

void	Cell::setWaterLevel(float w){ _w = w; }

void	Cell::setGroundLevel(float g){ _g = g; }



