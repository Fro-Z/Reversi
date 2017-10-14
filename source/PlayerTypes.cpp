#include "PlayerTypes.h"
std::string PlayerTypes::GetPlayerTypeDescription(PlayerType type)
{
	switch(type)
	{
		case PLAYER_LOCAL:
			return "Local player";
			break;
		case PLAYER_AI_SIMPLE:
			return "AI Simple";
			break;
		case PLAYER_AI_MINIMAX:
			return "AI Minimax";
			break;
		case PLAYER_REMOTE:
			return "Remote player";
			break;
		default:
			return "No description";
	}
}
