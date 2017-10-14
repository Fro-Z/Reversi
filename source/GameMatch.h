#ifndef GAMEMATCH_H_
#define GAMEMATCH_H_
#include "Player.h"
#include "GameMove.h"
#include "BoardData.h"
#include <vector>

/**
 * Class responsible for match logic, switching turns etc
 */
class GameMatch
{
public:
	GameMatch(Player* blackPlayer, Player* whitePlayer);
	virtual ~GameMatch();

	/// Do match logic
	virtual void Think();

	const BoardData& GetBoard() const;

	Player* GetTurningPlayer();

	/// Returns token type of turning player
	BoardData::CellState GetTurningPlayerToken() const;

	///Returns token type of opponent
	BoardData::CellState GetOpponentPlayerToken() const;

	bool HasEnded() const;

	///Get game score and victor as string
	std::string GetGameInfo() const;
protected:
	bool endGame = false;
	enum Players
	{
		PLAYER_BLACK,
		PLAYER_WHITE
	};

	Player* black; ///< Black player
	Player* white; ///< White player

	BoardData board; ///< Board for this game match

	int playerOnTurn; ///< Player on turn. 


	///Checks board for whether turning player can play this move
	bool IsMoveValid(const GameMove& move) const;

	///Ends the game after turning player can play no moves.
	virtual void EndGame();

	///Can the turning player make any moves
	bool IsPlayable() const;

	///Checks if board is playable and informs player of turn start
	virtual void StartTurn();

	///Changes turning player
	virtual void EndTurn();

private:
	GameMatch(const GameMatch&) = delete;
	GameMatch operator=(const GameMatch&) = delete;

};
#endif /* GAMEMATCH_H_ */
