#include <iostream>
#include "window.h"

using namespace std;

Window::Window()
{
}

//ANSI
void Window::Format(int color)
{
	int bg = 0;

	switch(color)
	{
		case 0: {bg = 0; break;} // Black
		case 1:	{bg = 41; break;} // Red
		case 2:	{bg = 42; break;} // Green
		case 3:	{bg = 43; break;} // Yellow
		case 4:	{bg = 44; break;} // Blue
		case 5:	{bg = 45; break;} // Magenta
		case 6:	{bg = 46; break;} // Cyan
		case 7:	{bg = 47; break;} // White
		case 8: {cout << "\033[48;2;255;165;0m"; return;} // Orange (RGB)
	}

	cout << "\033[0;" << bg << "m";

}



// a % b as positive
unsigned Window::Modulo(int a, unsigned b)
{
	int mod = a % (int)b;
	if (mod < 0)
		mod += b;
	return mod;
}


void Window::ShowCursor(bool visible)
{
	cout << "\033[?25" << (visible ? "h" : "l");
}

Window::~Window()
{
}