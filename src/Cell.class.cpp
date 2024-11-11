#include "Cell.class.hpp"

Cell::Cell() : _w(0), _totalVelocity(0), _vN(0), _vE(0), _vS(nullptr), _vW(nullptr) {}

Cell::Cell(float w, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell)
	: _w(w), _wN(nullptr), _wE(nullptr), _wS(nullptr), _wW(nullptr),
	_totalVelocity(0.0f) ,_vN(0.0f), _vE(0.0f), _vS(nullptr), _vW(nullptr)
{
	if (Ncell != nullptr)
		_wN = &(Ncell->_w);

	if (Ecell != nullptr)
		_wE = &(Ecell->_w);

	if (Scell != nullptr){
		_vS = &(Scell->_vN);
		_wS = &(Scell->_w);
	}

	if (Wcell != nullptr) {
		_vW = &(Wcell->_vE);
		_wW = &(Wcell->_w);
	}
}

Cell::~Cell() {}

Cell::Cell(const Cell& other)
{
	_w = other._w;
	_vN = other._vN;
	_vE = other._vE;
	_vS = other._vS;
	_vW = other._vW;

}

Cell& Cell::operator=(const Cell& other) {
	if (this != &other) {
		_w = other._w;
		_vN = other._vN;
		_vE = other._vE;
		if (other._vS != nullptr) {
			_vS = other._vS;
		} else {
			_vS = nullptr;
		}
		if (other._vW != nullptr) {
			_vW = other._vW;
		} else {
			_vW = nullptr;
		}
		if (other._wN != nullptr) {
			_wN = other._wN;
		} else {
			_wN = nullptr;
		}
		if (other._wE != nullptr) {
			_wE = other._wE;
		} else {
			_wE = nullptr;
		}
		if (other._wS != nullptr) {
			_wS = other._wS;
		} else {
			_wS = nullptr;
		}
		if (other._wW != nullptr) {
			_wW = other._wW;
		} else {
			_wW = nullptr;
		}
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

// Height = DELTA_TIME * (vN + vE + vS + vW)
	// _vS and _vW are inverted to get the right direction
void	Cell::calculateTotalVelocity() {
	_totalVelocity = _vN + _vE;
	_totalVelocity += _vS ? -*_vS : 0;
	_totalVelocity += _vW ? -*_vW : 0;
}

// void	Cell::resolveUnderflow() {
// 	if (_w >= 0)
// 		return;

// 	float	potential_credit = 0; // quantity of water higher than target that holds the neighbors

// 	if (_wN && *_wN > 0)
// 		potential_credit += *_wN;
// 	if (_wE && *_wE > 0)
// 		potential_credit += *_wE;
// 	if (_wS && *_wS > 0)
// 		potential_credit += *_wS;
// 	if (_wW && *_wW > 0)
// 		potential_credit += *_wW;
	
// }

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

void	Cell::updateHeight() {
	_w += DELTA_TIME * _totalVelocity;
}

float	Cell::getHeight() {
	return _w;
}

void	Cell::setHeight(float w){
	_w = w;
}

void	Cell::addToTotalVelocity(float add){
	_totalVelocity += add;
}
