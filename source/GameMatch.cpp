#include "GameMatch.h"
#include "GameMove.h"
#include "GameEngine.h"
#include <iostream>
#include <typeinfo>
#include <sstream>
#include "RemotePlayer.h"
#include "Network/PacketCommand.h"

GameMatch::GameMatch(Player* blackPlayer, Player* whitePlayer) :
		black(blackPlayer), white(whitePlayer), playerOnTurn(PLAYER_BLACK)
{
	//set default cell starting layout
	board.At(3, 3) = BoardData::CELL_WHITE;
	board.At(3, 4) = BoardData::CELL_BLACK;
	board.At(4, 3) = BoardData::CELL_BLACK;
	board.At(4, 4) = BoardData::CELL_WHITE;

	StartTurn();
}
GameMatch::~GameMatch()
{

}
void GameMatch::Think()
{
	if (endGame)
		return;

	Player* turningPlayer = GetTurningPlayer();

	if (turningPlayer->IsReady())
	{
		//Get player's move and check validity
		GameMove move = turningPlayer->GetMove();

		if (!IsMoveValid(move))
		{
			turningPlayer->RefuseMove();
		}
		else
		{
			board.DoMove(move, GetTurningPlayerToken());

			if (white->IsRemote()) //Inform player if he is remote
			{
				RemotePlayer* rp = dynamic_cast<RemotePlayer*>(white);
				rp->Send(PacketBoardData(board));
				rp->Send(PacketCommand(PacketCommand::COMMAND_NEXT_TURN));
			}

			EndTurn();
			StartTurn();
		}
	}
}
Player* GameMatch::GetTurningPlayer()
{
	if (playerOnTurn == PLAYER_WHITE)
		return white;
	else
		return black;
}
void GameMatch::EndTurn()
{
	if (playerOnTurn == PLAYER_WHITE) //change player on turn
		playerOnTurn = PLAYER_BLACK;
	else
		playerOnTurn = PLAYER_WHITE;
}
void GameMatch::StartTurn()
{
	if (IsPlayable())
	{
		GameEngine::GetInstance().OnStartTurn();
		GetTurningPlayer()->OnTurnBegin(*this);

	}
	else
		EndGame();
}
bool GameMatch::IsPlayable() const
{
	/* If any empty cell has player-owned "neighbour"
	 * it is a valid move for the player
	 */
	for (int i = 0; i < BoardData::BOARD_SIZE; i++)
	{
		for (int j = 0; j < BoardData::BOARD_SIZE; j++)
		{
			if (board.IsCellEmpty(GameMove(j, i)) && !board.FindClosestNeighbours(GameMove(j, i), GetTurningPlayerToken()).empty())
				return true;
		}
	}
	return false;
}
bool GameMatch::IsMoveValid(const GameMove& move) const
{
	if (board.IsCellEmpty(move) && !board.FindClosestNeighbours(move, GetTurningPlayerToken()).empty())
		return true;
	else
		return false;
}
void GameMatch::EndGame()
{
	endGame = true;
}
BoardData::CellState GameMatch::GetTurningPlayerToken() const
{
	return (playerOnTurn == PLAYER_WHITE) ? BoardData::CELL_WHITE : BoardData::CELL_BLACK;
}

BoardData::CellState GameMatch::GetOpponentPlayerToken() const
{
	return (playerOnTurn == PLAYER_WHITE) ? BoardData::CELL_BLACK : BoardData::CELL_WHITE;
}
const BoardData& GameMatch::GetBoard() const
{
	return board;
}
bool GameMatch::HasEnded() const
{
	return endGame;
}
std::string GameMatch::GetGameInfo() const
{
	std::stringstream ss;
	int blackScore = board.CountTokens(BoardData::CELL_BLACK);
	int whiteScore = board.CountTokens(BoardData::CELL_WHITE);

	if (endGame)
	{
		if (blackScore > whiteScore)
			ss << "BLACK wins";
		if (blackScore < whiteScore)
			ss << "WHITE wins";
		if (blackScore == whiteScore)
			ss << "It's a tie";
	}

	ss << " (" << blackScore << ":" << whiteScore << ")";
	return ss.str();
}
