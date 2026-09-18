#pragma once
#include "DxLib.h"

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Color() : r(255), g(255), b(255), a(255){}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
		: r(r), g(g), b(b), a(a) {}

	operator unsigned int() const
	{
		return GetColor(r, g, b);
	}

	// íËêîêF
	static Color White() { return Color(255, 255, 255); }
	static Color Black() { return Color(0, 0, 0); }
	static Color Red() { return Color(255, 0, 0); }
	static Color Blue() { return Color(0, 0, 255); }
	static Color Green() { return Color(0, 255, 0); }
	static Color Yellow() { return Color(255, 255, 0); }
	static Color Orange() { return Color(255, 165, 0); }
	static Color Purple() { return Color(128, 0, 128); }
	static Color Pink() { return Color(255, 105, 180); }
	static Color Cyan() { return Color(0, 255, 255); }
	static Color LimeGreen() { return Color(50, 255, 50); }
	static Color Gray() { return Color(128, 128, 128); }
	static Color LightGray() { return Color(200, 200, 200); }
	static Color DarkGray() { return Color(64, 64, 64); }
	static Color Brown() { return Color(139, 69, 19); }
	static Color Gold() { return Color(255, 215, 0); }

};
