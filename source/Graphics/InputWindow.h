#ifndef INPUTWINDOW_H_
#define INPUTWINDOW_H_
#include "Window.h"
#include "../KeyboardListener.h"
#include <string>
/**
 * Window for inputting text.
 * Displays an editable text with a cursor
 */
class InputWindow: public Window, public KeyboardListener
{
public:
	InputWindow(int posX, int posY, unsigned int textLength);
	virtual ~InputWindow();

	virtual void OnKeyPress(int key) override;
	virtual void Draw() override;

	///Get entered text as a string
	std::string GetText();
protected:
	const unsigned int maxTextLength;
	std::string text;
	std::string::iterator cursor;

	///Handle cursor moving with arrow keys
	void HandleCursorManipulation(int key);

	///Handle backspace and delete
	void HandleDeletion(int key);

	///adds character to the string
	void AddChar(char c);
};
#endif /* INPUTWINDOW_H_ */
