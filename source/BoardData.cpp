#include "BoardData.h"
#include <stdexcept>
BoardData::BoardData()
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			cell[i][j] = CELL_EMPTY;

}
void BoardData::CopyFrom(const BoardData& other)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			cell[i][j] = other.cell[i][j];
}
BoardData::BoardData(const BoardData& other)
{
	CopyFrom(other);
}
BoardData& BoardData::operator=(const BoardData& other)
{
	if (this == &other)
		return *this;

	CopyFrom(other);
	return *this;
}
bool BoardData::IsCellEmpty(const GameMove& move) const
{
	//check range
	if (move.x >= BOARD_SIZE || move.x < 0)
		return false;
	if (move.y >= BOARD_SIZE || move.y < 0)
		return false;

	if (cell[move.y][move.x] == CELL_EMPTY)
		return true;
	else
		return false;
}
std::vector<GameMove> BoardData::FindClosestNeighbours(const GameMove& origin, CellState playerToken) const
{
	std::vector<GameMove> neighbours;

	//find closest neighbours in lines (including diagonals)
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			FindClosestInLine(origin, playerToken, neighbours, i, j);

	return neighbours;
}
void BoardData::FindClosestInLine(const GameMove& origin, CellState playerToken, std::vector<GameMove>& neighbours, int horStep, int vertStep) const
{
	if (horStep == 0 && vertStep == 0)
		return;

	//setup token types
	auto opponentToken = ReverseToken(playerToken);

	//initialize at neighbouring cell
	GameMove move(origin);
	move.x += horStep;
	move.y += vertStep;
	bool connectedByOpponent = false; //origin and target must be connected by opponent's tokens

	for (;
			move.x < BoardData::BOARD_SIZE && move.x >= 0 //keep move coords within bounds
			&& move.y < BoardData::BOARD_SIZE && move.y >= 0
			;
			move.x += horStep, //increase both coords by step
			move.y += vertStep)
	{
		//action depends on encountered token type
		if (At(move) == playerToken) //same type token, neighbour only if connected via opponent's token
		{
			if (connectedByOpponent)
				neighbours.push_back(move);

			break; //do not search past first neighbour token
		}
		if (At(move) == BoardData::CELL_EMPTY)
		{
			break; //do not search through empty cells
		}
		if (At(move) == opponentToken)
		{
			connectedByOpponent = true;
		}
	}

}
BoardData::CellState& BoardData::At(int y, int x)
{
	if (y < 0 || y >= BOARD_SIZE)
		throw std::out_of_range("Move out of board range");
	if (x < 0 || x >= BOARD_SIZE)
		throw std::out_of_range("Move out of board range");
	return cell[y][x];
}
const BoardData::CellState& BoardData::At(int y, int x) const
{
	if (y < 0 || y >= BOARD_SIZE)
		throw std::out_of_range("Move out of board range");
	if (x < 0 || x >= BOARD_SIZE)
		throw std::out_of_range("Move out of board range");
	return cell[y][x];
}
BoardData::CellState& BoardData::At(const GameMove& move)
{
	return At(move.y, move.x);
}
const BoardData::CellState& BoardData::At(const GameMove& move) const
		{
	return At(move.y, move.x);
}
void BoardData::FillLine(const GameMove& origin, CellState playerToken, const GameMove& target)
{
	for (GameMove& location : GetLine(origin, target))
		At(location) = playerToken;
}
std::vector<GameMove> BoardData::GetLine(const GameMove& origin, const GameMove& target) const
{
	//calculate step required to go from origin to target
	auto getStep = [](int o, int t)
	{
		if(o<t) return 1;
		if(o>t) return -1;
		else return 0;
	};

	int xStep = getStep(origin.x, target.x);
	int yStep = getStep(origin.y, target.y);

	GameMove move(origin);
	std::vector<GameMove> result;
	while (move.x != target.x || move.y != target.y)
	{
		move.x += xStep;
		move.y += yStep;
		result.push_back(move);

		//update step
		xStep = getStep(move.x, target.x);
		yStep = getStep(move.y, target.y);
	}
	return result;
}
int BoardData::CountTokens(CellState tokenType) const
{
	int numOfTokens = 0;
	for (int y = 0; y < BOARD_SIZE; y++)
		for (int x = 0; x < BOARD_SIZE; x++)
			if (At(y, x) == tokenType)
				numOfTokens++;
	return numOfTokens;
}
void BoardData::DoMove(const GameMove& move, CellState playerToken)
{
	At(move) = playerToken;

	//find closest tokens of the same colour on each side and diagonal
	for (const GameMove& neighbour : FindClosestNeighbours(move, playerToken))
	{
		FillLine(move, playerToken, neighbour);
	}
}
BoardData::CellState BoardData::ReverseToken(CellState token)
{
	if (token == CELL_WHITE)
		return CELL_BLACK;

	if (token == CELL_BLACK)
		return CELL_WHITE;

	return token;
}
