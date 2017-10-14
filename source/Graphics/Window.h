#ifndef WINDOW_H_
#define WINDOW_H_
#include <ncurses.h>
#include "Drawable.h"
/**
 *  Wrapper class for ncurses window
 */
class Window: public Drawable
{
public:

	Window(int posX, int posY, int width, int height);
	virtual ~Window();

	///Draw window
	virtual void Draw() override;

	///Enable or disable border around window
	void SetBorder(bool enabled);
protected:
	void DrawBorder();
	void RemoveBorder();
	const int posX;
	const int posY;
	const int width;
	const int height;

	bool drawBorder;
	WINDOW* ncursesWin; //ncurses window
};
#endif /* WINDOW_H_ */
