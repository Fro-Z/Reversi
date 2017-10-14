#ifndef PLAYER_H_
#define PLAYER_H_
#include "GameMove.h"
//#include "GameMatchBase.h"
#include <iostream>

/**
 *  Player base class
 */
class Player
{
public:
	Player(bool remote,const std::string& name);
	virtual ~Player();

	///Is ready to provide the next move?
	virtual bool IsReady() const;

	///Returns the move for this turn
	virtual GameMove GetMove() const = 0;

	///Called by engine to discard currently offered move (not valid move)
	virtual void RefuseMove() = 0;

	///Called when player's turn starts
	virtual void OnTurnBegin(const class GameMatch& match) = 0;

	const std::string& GetName() const;

	bool IsRemote() const;

protected:
	const bool remote;
	const std::string name;
};



#endif /* PLAYER_H_ */
