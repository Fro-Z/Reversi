#ifndef MOVEINPUTWINDOW_H_
#define MOVEINPUTWINDOW_H_
#include "InputWindow.h"
#include "../GameMove.h"

/**
 * Input window that allows only valid move entries
 * Checks only if the format is valid, does not check for free space on the board
 *
 */
class MoveInputWindow: public InputWindow
{
public:
	MoveInputWindow(int posX, int posY);
	virtual ~MoveInputWindow();

	virtual void Draw() override;
	/**
	 *  Accepts only input within specific range and only until confirmation
	 */
	virtual void OnKeyPress(int key) override;

	///Is input valid and can be read?
	bool IsInputConfirmed();

	///Reset input box and state
	void Reset();

	GameMove GetMove();
private:
	void UpdateText();
	bool confirmed;
	GameMove move;

	///LineFeed ASCII value
	static const int KEY_LF = 10;
};
#endif /* MOVEINPUTWINDOW_H_ */
