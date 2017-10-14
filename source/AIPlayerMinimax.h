#ifndef AIPLAYERMINIMAX_H_
#define AIPLAYERMINIMAX_H_
#include "Player.h"
#include "BoardData.h"
#include <climits>
#include <list>

/**
 * AI player using the minimax algorithm with alpha-beta pruning
 */
class AIPlayerMinimax: public Player
{
public:
	AIPlayerMinimax(const std::string& name);
	virtual ~AIPlayerMinimax();

	///Is ready to provide the next move?
	virtual bool IsReady() const override;

	///Returns the move for this turn
	virtual GameMove GetMove() const override;

	///Called by engine to discard currently offered move (not valid move)
	virtual void RefuseMove() override;

	///Called when player's turn starts
	virtual void OnTurnBegin(const class GameMatch& match) override;
private:
	/// Representation of possible move and resulting state
	struct Node
	{
		Node(int depth, const GameMove& move, const BoardData& b) :
				type(true), expanded(false), score(0), alpha(INT_MIN), beta(INT_MAX),
				depth(depth), entryMove(move), board(b), parent(nullptr)
		{}
		~Node()
		{
			for (auto nodeptr : children)
				delete nodeptr;
		}

		bool type; ///< Node type: false==MIN true==MAX
		bool expanded; ///< Was this node expanded?

		int score; ///< Node score

		///used for alpha beta pruning
		int alpha; ///< Alpha value used for pruning. (Highest score of a MAX node)
		int beta; ///< Beta value used for pruning. (Lowest score of a MIN node)

		int depth; ///< Node depth. Equal to amount of turns required -1

		GameMove entryMove; ///< Move used to get to Node's board state
		BoardData board; ///< Board result of the move

		std::list<Node*> children; ///< List of possible future board states
		Node* parent; ///< Source board state

		///Updates parent alpha and beta values based on current values, score and type
		void UpdatePruneValues();

		/// Set current alpha and update node children
		void SetAlpha(int newAlpha);

		/// Set current beta and update node children
		void SetBeta(int newBeta);

	};

	///Calculate next move
	void CalculateMove(BoardData::CellState playerTokenType, const BoardData& board);

	/**
	 * Calculate score for future move node
	 * @param board Current board
	 * @param node Node containing future board state
	 * @param currentToken Turning player's (of future turn) token type
	 */
	int CalcScore(const BoardData& board, const Node & node, BoardData::CellState currentToken) const;

	///Update nodes score depending on node type and children nodes
	void UpdateScore(Node& node) const;

	///Finds all possible moves on board
	std::vector<GameMove> GetAllMoves(BoardData::CellState playerTokenType, const BoardData& board) const;

	///Prevents node expansion and sets score that prevents node selection
	void Discard(Node*) const;

	GameMove calculatedMove; ///< Move calculated using the minimax algorithm

	bool ready; ///< Is AI ready to provide the move
	static const int SEARCH_DEPTH = 4; ///< How many turns in future should be searched for best move
};

#endif /* AIPLAYERMINIMAX_H_ */
