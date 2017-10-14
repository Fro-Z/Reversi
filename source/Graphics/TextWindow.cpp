#include "TextWindow.h"
TextWindow::TextWindow(const char* text, int posX, int posY, int width, int height) :
		Window(posX, posY, width, height), text(text)
{

}
TextWindow::~TextWindow()
{

}
void TextWindow::SetText(const char* newText)
{
	text = std::string(newText);
	for (int y = 1; y < height; y++)
		for (int x = 1; x < width; x++)
			mvwaddch(ncursesWin, y, x, ' '); //repaint with whitespace

}
void TextWindow::Draw()
{
	Window::Draw(); //draws border

	int horizontalPadding = ((width - static_cast<int>(text.length())) / 2);
	int verticalPadding = (height - 1) / 2;

	//clamp values (0 - x)
	verticalPadding = verticalPadding > 0 ? verticalPadding : 0;
	horizontalPadding = horizontalPadding > 0 ? horizontalPadding : 0;

	int displayedLength = static_cast<int>(text.length());
	if (static_cast<int>(text.length()) > width - 2) 	//border takes one char from each side
		displayedLength = width - 2;

	std::string shortText = text;
	if (displayedLength < static_cast<int>(shortText.length())) //end string at displayedLength if needed
		shortText.at(displayedLength) = 0;

	mvwprintw(ncursesWin, verticalPadding, horizontalPadding, shortText.c_str());

	wrefresh(ncursesWin);
}
