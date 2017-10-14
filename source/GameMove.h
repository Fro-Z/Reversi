#ifndef GAMEMOVE_H_
#define GAMEMOVE_H_
#include <iostream>
/**
 * Coordinate pair defining a game move
 */
struct GameMove
{
	GameMove():x(0),y(0)
	{}
	GameMove(int x, int y):x(x),y(y)
	{}
	GameMove(const GameMove& other):x(other.x),y(other.y)
	{}
	GameMove& operator=(const GameMove& other)
	{
		x=other.x;
		y=other.y;
		return *this;
	}

	int x;
	int y;
};


#endif /* GAMEMOVE_H_ */
