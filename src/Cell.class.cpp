#include "Cell.class.hpp"

Cell::Cell() : _h(0), _vN(0), _vE(0), _vS(nullptr), _vW(nullptr) {}

Cell::Cell(float h, Cell* Ncell, Cell* Ecell, Cell* Scell, Cell* Wcell)
	: _h(h), _hN(nullptr), _hE(nullptr), _hS(nullptr), _hW(nullptr),
	_vN(0.0f), _vE(0.0f), _vS(nullptr), _vW(nullptr)
{
	if (Ncell != nullptr)
		_hN = &(Ncell->_h);

	if (Ecell != nullptr)
		_hE = &(Ecell->_h);

	if (Scell != nullptr){
		_vS = &(Scell->_vN);
		_hS = &(Scell->_h);
	}

	if (Wcell != nullptr) {
		_vW = &(Wcell->_vE);
		_hW = &(Wcell->_h);
	}
}

Cell::~Cell() {}

Cell::Cell(const Cell& other)
{
	_h = other._h;
	_vN = other._vN;
	_vE = other._vE;
	_vS = other._vS;
	_vW = other._vW;

}

Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        _h = other._h;
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
        if (other._hN != nullptr) {
            _hN = other._hN;
        } else {
            _hN = nullptr;
        }
        if (other._hE != nullptr) {
            _hE = other._hE;
        } else {
            _hE = nullptr;
        }
        if (other._hS != nullptr) {
            _hS = other._hS;
        } else {
            _hS = nullptr;
        }
        if (other._hW != nullptr) {
            _hW = other._hW;
        } else {
            _hW = nullptr;
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////


// Acceleration = PROPAGATION * (neighbor.h - target.h)
// Velocity = (DAMPENING * target.v) + (DELTA_TIME * target.a)
void	Cell::updateVelocity() {

	if (_hN != nullptr)
		_vN = (DAMPENING * _vN) + (DELTA_TIME * PROPAGATION * (*_hN - _h));
	else
		_vN = 0;

	if (_hE != nullptr)
		_vE = (DAMPENING * _vE) + (DELTA_TIME * PROPAGATION * (*_hE - _h));
	else
		_vE = 0;
}

// Height = DELTA_TIME * (vN + vE + vS + vW)
	// _vS and _vW are inverted to get the right direction
void	Cell::updateHeight() {
	float	totalVelocity;

	totalVelocity = _vN + _vE;
	totalVelocity += _vS ? -*_vS : 0;
	totalVelocity += _vW ? -*_vW : 0;

	_h += DELTA_TIME * totalVelocity;
}

float	Cell::getHeight() {
	return _h;
}

void	Cell::setHeight(float h){
	_h = h;
}
