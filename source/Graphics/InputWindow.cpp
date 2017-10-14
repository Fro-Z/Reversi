#include "InputWindow.h"
#include "../GameEngine.h"
#include <iostream>
InputWindow::InputWindow(int posX, int posY, unsigned int textLength) :
		Window(posX, posY, 2 + textLength, 3), maxTextLength(textLength), cursor(text.begin())
{

}
InputWindow::~InputWindow()
{
	//remove KeyboardListener if remained active
	GameEngine::GetInstance().RemoveKeyboardListener(this); 
}
void InputWindow::OnKeyPress(int key)
{
	HandleCursorManipulation(key);
	HandleDeletion(key);

	if (key >= 32 && key <= 126) //accept ASCII without control chars and DEL
	{
		AddChar(static_cast<char>(key));
		SetDirty(true);
	}
}
void InputWindow::Draw()
{
	Window::Draw();

	//draw text
	std::string::iterator printedChar = text.begin();
	for (unsigned int i = 0; i < maxTextLength; i++)
	{
		if (printedChar == cursor) //underline cursor position
		{
			wattron(ncursesWin, A_UNDERLINE);

		}

		if (i < text.length()) //print text or fill with whitespace
			mvwaddch(ncursesWin, 1, 1 + i, text.at(i));
		else
			mvwaddch(ncursesWin, 1, 1 + i, ' ');

		if (printedChar == cursor)
		{
			wattroff(ncursesWin, A_UNDERLINE);
		}

		printedChar++;

	}

	wrefresh(ncursesWin);
}
void InputWindow::HandleCursorManipulation(int key)
{
	if (key == KEY_LEFT)
	{
		//move cursor to the left
		if (cursor == text.begin())
		{
			cursor = text.end();
			if (text.length() == maxTextLength)
				cursor--; //text.end would be out of the box
		}
		else
			cursor--;

		SetDirty(true);
		return;
	}
	if (key == KEY_RIGHT)
	{
		//move cursor to the right
		if (cursor == text.end())
			cursor = text.begin();
		else
		{
			cursor++;
			if (cursor == text.end() && text.length() == maxTextLength) //text.end would be out of the box
				cursor = text.begin();
		}

		SetDirty(true);
		return;
	}
}
std::string InputWindow::GetText()
{
	return text;
}
void InputWindow::AddChar(char c)
{
	if (text.length() < maxTextLength)
	{
		//note: iterator gets invalidated after insertion before it's location
		int distInText = static_cast<int>(std::distance(text.begin(), cursor));
		text.insert(cursor, c);
		cursor = text.begin() + distInText;
		cursor++;

	}
}
void InputWindow::HandleDeletion(int key)
{
	if (key == KEY_DC)
	{ //delete char at cursor, preserve cursor position
		if (cursor != text.end())
		{
			if (cursor == text.begin())
			{
				text.erase(cursor);
				cursor = text.begin();
			}
			else
			{
				int distInText = static_cast<int>(std::distance(text.begin(), cursor));
				text.erase(cursor);
				cursor = text.begin() + distInText;
			}

			SetDirty(true);
		}
	}
	if (key == KEY_BACKSPACE)
	{ //delete char before cursor, preserve cursor position
		if (cursor != text.begin())
		{

			auto previous = cursor - 1;
			if (previous == text.begin())
			{
				text.erase(cursor - 1);
				cursor = text.begin();
			}
			else
			{
				previous--; //move iterator before deleted position
				text.erase(cursor - 1);
				cursor = previous + 1;

			}
		}

		SetDirty(true);
	}
}
