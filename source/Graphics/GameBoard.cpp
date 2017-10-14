#include "GameBoard.h"
GameBoard::GameBoard(int posX, int posY, const BoardData* board, const GameMatch* match) :
		Window(posX, posY, windowSizeX, windowSizeY),
		board(board), match(match)
{

}
GameBoard::~GameBoard()
{

}
void GameBoard::UpdateBoard()
{
	SetDirty(true);
}
void GameBoard::Draw()
{
	SetDirty(false);

	if (board == nullptr)
		return;

	for (int i = 0; i < BoardData::BOARD_SIZE; i++) //row
	{
		for (int j = 0; j < BoardData::BOARD_SIZE; j++) //column
		{
			DrawCell(j, i);
		}
	}

	wrefresh(ncursesWin);
}
void GameBoard::DrawCell(int cellX, int cellY)
{
	SetColorFromCell(cellX, cellY);

	int startPosX = cellX * cellSizeX;
	int startPosY = cellY * cellSizeY;

	wmove(ncursesWin, startPosY, startPosX); //top horizontal line
	waddch(ncursesWin, ACS_ULCORNER);
	for (int i = 1; i < cellSizeX - 1; i++)
		waddch(ncursesWin, ACS_HLINE);
	waddch(ncursesWin, ACS_URCORNER);

	wmove(ncursesWin, startPosY + cellSizeY - 1, startPosX); //bottom horizontal line
	waddch(ncursesWin, ACS_LLCORNER);
	for (int i = 1; i < cellSizeX - 1; i++)
		waddch(ncursesWin, ACS_HLINE);
	waddch(ncursesWin, ACS_LRCORNER);

	for (int i = 1; i < cellSizeY - 1; i++) //left vertical line
		mvwaddch(ncursesWin, startPosY + i, startPosX, ACS_VLINE);

	for (int i = 1; i < cellSizeY - 1; i++) //right vertical line
		mvwaddch(ncursesWin, startPosY + i, startPosX + cellSizeX - 1, ACS_VLINE);

	PrintCellText(cellX, cellY);
}
void GameBoard::PrintCellText(int x, int y)
{
	int startPosX = x * cellSizeX + 1;
	int startPosY = y * cellSizeY + 1;
	wmove(ncursesWin, startPosY, startPosX);
	waddch(ncursesWin, 'A' + x);
	waddch(ncursesWin, '0' + y);
	waddch(ncursesWin, ' ');
}
void GameBoard::SetColorFromCell(int x, int y)
{
	switch (board->At(y, x))
	{
		case BoardData::CellState::CELL_EMPTY:
			//--Highlight possible moves
			if (!board->FindClosestNeighbours(GameMove(x, y), match->GetTurningPlayerToken()).empty())
				wattron(ncursesWin, COLOR_PAIR(4));
			else
				wattron(ncursesWin, COLOR_PAIR(1));

			break;
		case BoardData::CellState::CELL_BLACK:
			wattron(ncursesWin, COLOR_PAIR(2));
			break;
		case BoardData::CellState::CELL_WHITE:
			wattron(ncursesWin, COLOR_PAIR(3));
			break;
		default:
			wattron(ncursesWin, COLOR_PAIR(1));
	}
}
