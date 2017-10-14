#ifndef TEXTWINDOW_H_
#define TEXTWINDOW_H_
#include <string>
#include "Window.h"
/**
 *  @brief Window containing text string
 *
 *  String is centered horizontally and vertically
 *  String longer than window's width will not scale or overflow the window
 */
class TextWindow: public Window
{
public:
	TextWindow(const char* text,int posX, int posY, int width, int height);
	virtual ~TextWindow();

	void SetText(const char* newText);

	virtual void Draw() override;
private:
	std::string text;
};
#endif /* TEXTWINDOW_H_ */
