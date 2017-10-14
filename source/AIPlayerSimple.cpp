#include "AIPlayerSimple.h"
#include "GameMatch.h"
#include <iostream>

AIPlayerSimple::AIPlayerSimple(const std::string& name) :
		Player(false, name), calculatedMove(0, 0), ready(false)
{

}
AIPlayerSimple::~AIPlayerSimple()
{

}
bool AIPlayerSimple::IsReady() const
{
	return ready;
}
GameMove AIPlayerSimple::GetMove() const
{
	return calculatedMove;
}
void AIPlayerSimple::RefuseMove()
{
	std::cerr << "\nERROR: AI move was refused!";
}
void AIPlayerSimple::OnTurnBegin(const GameMatch& match)
{
	const GameMatch& gmatch = dynamic_cast<const GameMatch&>(match);
	CalculateMove(gmatch.GetTurningPlayerToken(), gmatch.GetBoard());
}
void AIPlayerSimple::CalculateMove(BoardData::CellState playerTokenType, const BoardData& board)
{
	/*
	 * Find all valid moves and choose random between the highest values
	 */
	int bestMoveScore = 0;
	std::vector<GameMove> bestMoves;

	for (int y = 0; y < BoardData::BOARD_SIZE; y++)
	{
		for (int x = 0; x < BoardData::BOARD_SIZE; x++)
		{
			GameMove moveToTest(x, y);

			if (board.IsCellEmpty(moveToTest))
			{
				int moveScore = 0;
				for (auto neighbour : board.FindClosestNeighbours(moveToTest, playerTokenType))
				{
					moveScore += static_cast<int>(board.GetLine(moveToTest, neighbour).size()); //score increases for each token converted
				}

				if (moveScore > bestMoveScore) //keep only the best moves
				{
					bestMoves.clear();
					bestMoveScore = moveScore;
				}
				if (moveScore == bestMoveScore)
					bestMoves.push_back(moveToTest);
			}

		}
	}
	if (bestMoves.size() == 0)
	{
		std::cerr << "\nERROR!: AI SIMPLE did not find any move";
		exit(0);
	}

	int choice = static_cast<int>(rand() % bestMoves.size());
	calculatedMove = bestMoves.at(choice);
	ready = true;

}
