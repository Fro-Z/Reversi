#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#include "Window.h"
#include "../BoardData.h"
#include "../GameMatch.h"
/**
 * GameBoard class used for displaying BoardData
 */
class GameBoard: public Window
{
public:
	GameBoard(int posX, int posY,const BoardData* board,const GameMatch* match);
	virtual ~GameBoard();

	/// Must be called after outside change to the board is made to properly draw the changed board
	void UpdateBoard();

	///Draw the board in terminal
	virtual void Draw() override;
private:
	const BoardData* board;
	const GameMatch* match;
	static const int cellSizeX = 5;
	static const int cellSizeY = 3;
	static const int windowSizeX = BoardData::BOARD_SIZE*cellSizeX;
	static const int windowSizeY = BoardData::BOARD_SIZE*cellSizeY;

	///Draws single cell
	void DrawCell(int cellX, int cellY);

	///Set curses color pair by cell token color
	void SetColorFromCell(int x, int y);

	void PrintCellText(int x, int y);
};
#endif /* GAMEBOARD_H_ */
