#ifndef KEYBOARDLISTENER_H_
#define KEYBOARDLISTENER_H_

/**
 * 	@brief Abstract KeyboardListener interface
 *
 *	Objects of this type can listen for keyboard press events
 * 	KeyboardListener must be registered in GameEngine
 */
class KeyboardListener
{
public:
	virtual ~KeyboardListener();

	///Called after every key press
	virtual void OnKeyPress(int key) = 0;

};
#endif /* KEYBOARDLISTENER_H_ */
