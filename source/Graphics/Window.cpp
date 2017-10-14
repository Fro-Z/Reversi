#include "Window.h"

Window::Window(int posX, int posY, int width, int height):
posX(posX),posY(posY),width(width),height(height),drawBorder(true),ncursesWin(nullptr)
{
	ncursesWin=newwin(height, width, posY,posX);
}

Window::~Window()
{
	delwin(ncursesWin);

	if(GetParent()) //parent needs to redraw to hide this window
		GetParent()->SetDirty();
}
void Window::Draw()
{
	if(drawBorder)
		DrawBorder();
	wrefresh(ncursesWin);

	SetDirty(false); //drawing clears dirty state
}
void Window::DrawBorder()
{
	box(ncursesWin,0,0);
}
void Window::SetBorder(bool enabled)
{
	drawBorder=enabled;

	if(!drawBorder)
		RemoveBorder(); //border needs to be repained with whitespace to disappear

	SetDirty(true);
}
void Window::RemoveBorder()
{
	wborder(ncursesWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
}
