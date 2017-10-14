#include "KeyboardListener.h"
#include "GameEngine.h"

KeyboardListener::~KeyboardListener()
{
	GameEngine::GetInstance().RemoveKeyboardListener(this);
}
