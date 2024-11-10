#include "Renderer2D.class.hpp"

Renderer2D::Renderer2D(int width, int height)
	: _width(width), _height(height)
{

}

Renderer2D::~Renderer2D()
{

}

Renderer2D::Renderer2D(const Renderer2D& other)
{
	(void)other;
	// _value = other.value;
	// _ptr = new int;
	// *_ptr = *source._ptr;
}

Renderer2D& Renderer2D::operator=(const Renderer2D& other)
{
	if (this != &other)
	{
		// _value = other.value;

		// delete  _ptr;
		// _ptr = new int;
		// *_ptr = *source._ptr;
	}
	return (*this);
}

void	Renderer2D::update(std::vector<Cell> cells) {

	int i = 0;
	
	_waterHeight.resize(cells.size());
	for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); it++, i++) {
		_waterHeight[i] = it->h;
	}

}

void	Renderer2D::displayASCII() {
	float	min = 0.0f;
	float	max = 1.5f;
	int		color;
	float	delta = (max - min) / 24.0f;

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			color = floor(_waterHeight[x + y * _width] / delta);
			if (color < 0)
				color = -36;
			else if (color > 23)
				color = 23;
			std::cout << "\033[48;5;" << color + 232 << "m" << "  " << "\033[0m";
		}
		std::cout << std::endl;
	}

	// for (int i = 0; i < 24; i++) {
	// 	std::cout << "\033[48;5;" << i + 232 << "m" << "Color " << i << std::endl;
	// }
}