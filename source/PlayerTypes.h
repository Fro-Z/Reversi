#ifndef PLAYERTYPES_H_
#define PLAYERTYPES_H_
#include <iostream>
enum PlayerType
{
	PLAYER_LOCAL=0,
	PLAYER_AI_SIMPLE=1,
	PLAYER_AI_MINIMAX=2,
	PLAYER_REMOTE=3
};
namespace PlayerTypes
{
	///Returns text description of a player type
	std::string GetPlayerTypeDescription(PlayerType type);
}

#endif /* PLAYERTYPES_H_ */
