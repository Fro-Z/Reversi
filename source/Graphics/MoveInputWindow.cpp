#include "MoveInputWindow.h"
#include "../GameEngine.h"
#include "../BoardData.h"

MoveInputWindow::MoveInputWindow(int posX, int posY) :
		InputWindow(posX, posY, 3), confirmed(false)
{
	GameEngine::GetInstance().AddKeyboardListener(this);
	text = "A-0";
}
MoveInputWindow::~MoveInputWindow()
{
	GameEngine::GetInstance().RemoveKeyboardListener(this);
}

void MoveInputWindow::Draw()
{
	InputWindow::Draw();
}
bool MoveInputWindow::IsInputConfirmed()
{
	return confirmed;
}
void MoveInputWindow::Reset()
{
	confirmed = false;
	move.x = 0;
	move.y = 0;
	UpdateText();
}
GameMove MoveInputWindow::GetMove()
{

	return GameMove(move);
}
void MoveInputWindow::OnKeyPress(int key)
{
	/*
	 * Valid entries:
	 * 65- 65+BS	A-Z ASCII
	 * 97 - 97+BS	a-z ASCII
	 * 48-57 numbers
	 */

	if (confirmed) //do not accept input after confirming
		return;

	if (key >= '0' && key <= '9') //ASCII numbers
	{
		move.y = key - '0'; //vertical coordinate
		SetDirty(true);

	}

	if (key >= 'A' && key <= 'A' + BoardData::BOARD_SIZE)
	{
		move.x = key - 'A'; //horizontal coordinate
	}
	if (key >= 'a' && key <= 'a' + BoardData::BOARD_SIZE)
	{
		move.x = key - 'a'; //horizontal coordinate
	}

	if (key == KEY_ENTER || key == KEY_LF)
	{
		confirmed = true;
	}

	UpdateText();
	SetDirty(true);
}
void MoveInputWindow::UpdateText()
{
	//format:   D-4
	text.at(0) = static_cast<char>(65 + move.x);
	text.at(2) = static_cast<char>(48 + move.y);
}
