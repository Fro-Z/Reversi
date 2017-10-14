#ifndef AIPLAYERSIMPLE_H_
#define AIPLAYERSIMPLE_H_
#include "Player.h"
#include "BoardData.h"
#include <stdlib.h> //rand
/**
 * Simple AI.
 * Chooses always the move with highest token gain
 */
class AIPlayerSimple: public Player
{
public:
	AIPlayerSimple(const std::string& name);
	virtual ~AIPlayerSimple();

	///Is ready to provide the next move?
	virtual bool IsReady() const override;

	///Returns the move for this turn
	virtual GameMove GetMove() const override;

	///Called by engine to discard currently offered move (not valid move)
	virtual void RefuseMove() override;

	///Called when player's turn starts
	virtual void OnTurnBegin(const class GameMatch& match) override;
private:
	///Calculate next move
	void CalculateMove(BoardData::CellState playerTokenType,const BoardData& board);
	GameMove calculatedMove; ///< Move calculated using the CalculateMove method
	bool ready;

};
#endif /* AIPLAYERSIMPLE_H_ */
