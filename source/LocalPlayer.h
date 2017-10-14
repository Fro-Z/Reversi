#ifndef LOCALPLAYER_H_
#define LOCALPLAYER_H_
#include "Graphics/MoveInputWindow.h"
#include "Player.h"
/**
 *  Local human-controlled player class
 */
class LocalPlayer: public Player
{
public:
	LocalPlayer(const std::string& name);
	virtual ~LocalPlayer();

	///Is ready to provide the next move?
	virtual bool IsReady() const override;

	///Returns the move for this turn
	virtual GameMove GetMove() const override;

	///Called by engine to discard currently offered move (not valid move)
	virtual void RefuseMove() override;

	///Called when player's turn starts
	virtual void OnTurnBegin(const class GameMatch& match) override;

	///Set input window where player gets his turns from
	void SetMoveInputWin(MoveInputWindow* win);
private:
	MoveInputWindow* inputWin;
};


#endif /* LOCALPLAYER_H_ */
