#ifndef MAINMENU_H_
#define MAINMENU_H_
#include "Drawable.h"
#include "TextWindow.h"
#include "InputWindow.h"
#include "../KeyboardListener.h"
#include "../PlayerTypes.h"
#include <vector>
#include <memory>

/**
 * Interactive main menu Drawable
 */

class MainMenu: public Drawable, public KeyboardListener
{
public:
	MainMenu();
	~MainMenu();

	virtual void OnKeyPress(int key) override;
	virtual void Draw() override;
private:
	int selection; ///<Currently selected field

	///@{
	///Selectable menu fields
	TextWindow* w_startGame;
	TextWindow* w_player1;
	TextWindow* w_player2;
	TextWindow* w_joinGame;
	InputWindow* w_joinGameIP;
	///@}
	enum selections ///< Selectable menu fields
	{
		SELECT_STARTGAME=0,
		SELECT_PLAYER1=1,
		SELECT_PLAYER2=2,
		SELECT_JOINGAME=3,
		SELECT_JOINGAME_IP=4
	};

	static const int TERM_WIDTH = 80; //default terminal width in most environments
	int playerType1;
	int playerType2;

	std::vector<std::unique_ptr<Window>> windows;

	///Display border only around selected window
	void UpdateBorder();

	///Updates playerType selection text
	void UpdateText();

};
#endif /* MAINMENU_H_ */
