#ifndef BOARDDATA_H_
#define BOARDDATA_H_
#include "GameMove.h"
#include <vector>

///Contains information about tokens on board and provides board manipulation functions
class BoardData
{
public:
	static const int BOARD_SIZE = 8; ///< Size of the game board in cells

	/// Type of token in the cell
	enum CellState
	{
		CELL_EMPTY = 0,
		CELL_BLACK = 1,
		CELL_WHITE = 2
	};

	BoardData();
	BoardData(const BoardData& other);
	BoardData& operator=(const BoardData& other);
	bool IsCellEmpty(const GameMove& move) const; ///< Is cell in range and empty?

	///@{
	///Get reference to cell at location. Checks boundaries.
	BoardData::CellState& At(int y, int x);
	BoardData::CellState& At(const GameMove& move);
	const BoardData::CellState& At(int y, int x) const;
	const BoardData::CellState& At(const GameMove& move) const;
	///@}

	/**
	 * @brief Find closest connected player owned tokens on each side and diagonal.
	 * Tokens are only accepted if they are connected by opponent's tokens.
	 */
	std::vector<GameMove> FindClosestNeighbours(const GameMove& origin, CellState playerToken) const;
	/**
	 * @brief Finds closest token on side specified by params
	 * Tokens are only accepted if they are connected by opponent's tokens
	 * @param origin Line start
	 * @param horStep Horizontal increment in each loop
	 * @param vertStep Vertical increment in each loop
	 * @param neighbours Output param for found neighbour (if any)
	 */
	void FindClosestInLine(const GameMove& origin, CellState playerToken, std::vector<GameMove>& neighbours, int horStep, int vertStep) const;

	/**
	 * Fills a line between two moves with tokens of specified type
	 * @param origin Line origin.
	 * @param target Line target.
	 */
	void FillLine(const GameMove& origin, CellState playerToken, const GameMove& target);

	///Gets all cells in line between origin and target
	std::vector<GameMove> GetLine(const GameMove& origin, const GameMove& target) const;

	///Returns the number of specified tokens on board
	int CountTokens(CellState tokenType) const;

	/**
	 *  Perform a game move by turning player
	 *  @param move A valid move
	 */
	void DoMove(const GameMove& move, CellState playerToken);

	/// Return opposite token type
	static CellState ReverseToken(CellState token);
private:
	CellState cell[BOARD_SIZE][BOARD_SIZE];

	///Copy constructor and assignment operator helper function
	void CopyFrom(const BoardData& other);

};
#endif /* BOARDDATA_H_ */
