#include "AIPlayerMinimax.h"
#include "GameMatch.h"
#include <stack>

//ai waiting
#include <chrono>
#include <thread>

AIPlayerMinimax::AIPlayerMinimax(const std::string& name) :
		Player(false, name), ready(false)
{

}
AIPlayerMinimax::~AIPlayerMinimax()
{

}
bool AIPlayerMinimax::IsReady() const
{
	return ready;
}
GameMove AIPlayerMinimax::GetMove() const
{
	//add artificial delay to better see AI move
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	return calculatedMove;
}
void AIPlayerMinimax::RefuseMove()
{
	std::cerr << "\nERROR: AI move was refused!";
}
void AIPlayerMinimax::OnTurnBegin(const GameMatch& match)
{
	const GameMatch& gmatch = dynamic_cast<const GameMatch&>(match);
	CalculateMove(gmatch.GetTurningPlayerToken(), gmatch.GetBoard());
}
void AIPlayerMinimax::CalculateMove(BoardData::CellState playerTokenType, const BoardData& board)
{
	BoardData::CellState opponentTokenType = BoardData::CELL_WHITE;
	if (playerTokenType == BoardData::CELL_WHITE)
		opponentTokenType = BoardData::CELL_BLACK;

	//Depth first expansion and evaluation of possible moves
	std::stack<Node*> nodeStack;

	Node* root = new Node(0, GameMove(), board);

	nodeStack.push(root);

	while (!nodeStack.empty())
	{
		Node* current = nodeStack.top();
		//players alternate turns, on MAX nodes AIPlayerMinimax is on turn
		BoardData::CellState currentTokenType = (current->type ? playerTokenType : opponentTokenType);

		if (current->alpha >= current->beta) //alpha-beta pruning, discard node if values overlap
		{
			Discard(current);
			nodeStack.pop();
			continue;
		}

		//if not in target depth expand the node
		if (current->depth < SEARCH_DEPTH && !current->expanded)
		{
			/*NOTE:If node cannot be expanded(no possible moves) the node remains on the stack
			 *  with expanded state	and score is calculated in the next loop*/

			for (GameMove& move : GetAllMoves(currentTokenType, current->board))
			{
				Node* moveResult = new Node(current->depth + 1, move, current->board);

				moveResult->alpha = current->alpha; //inherit prune values from parent
				moveResult->beta = current->beta;

				moveResult->board.DoMove(move, currentTokenType); //update resulting board of the node
				moveResult->type = !(current->type); //alternate types between depths
				nodeStack.push(moveResult);

				current->children.push_back(moveResult); //the newly created node is child of current node
				moveResult->parent = current;
			}
			current->expanded = true;
		}
		else //if node is expanded or in target depth evaluate
		{
			if (current->children.empty())
			{
				current->score = CalcScore(board, *current, currentTokenType);
			}
			else
				UpdateScore(*current);

			current->UpdatePruneValues();

			nodeStack.pop();
		}
	}

	//select best move
	int bestScore = INT_MIN;
	for (Node* candidate : root->children)
	{
		if (candidate->score > bestScore)
		{
			bestScore = candidate->score;
			calculatedMove = candidate->entryMove;
		}
	}
	delete root;
	ready = true;
}
void AIPlayerMinimax::Discard(Node* node) const
{
	if (node->parent)
		node->parent->children.remove(node);
	delete node;
}
void AIPlayerMinimax::Node::UpdatePruneValues()
{
	if (!parent)
		return;

	if (type) //current type is MAX, parent type is MIN
	{
		//try to lower beta value
		if (parent->beta > score)
			parent->SetBeta(score);
	}
	else //current type is MIN, parent type is MAX
	{
		//try to increase alpha value
		if (parent->alpha < score)
			parent->SetAlpha(score);

	}
}
void AIPlayerMinimax::Node::SetAlpha(int newAlpha)
{
	alpha = newAlpha;
	for (auto nodeptr : children)
		nodeptr->alpha = newAlpha;
}
void AIPlayerMinimax::Node::SetBeta(int newBeta)
{
	beta = newBeta;
	for (auto nodeptr : children)
		nodeptr->beta = newBeta;
}
void AIPlayerMinimax::UpdateScore(Node& node) const
{
	int max = INT_MIN; //initialize values at worst possible score
	int min = INT_MAX;

	for (Node* childPtr : node.children)
	{
		if (childPtr->score > max)
			max = childPtr->score;

		if (childPtr->score < min)
			min = childPtr->score;
	}

	if (node.type) //true for MAX nodes, false for MIN nodes
		node.score = max;
	else
		node.score = min;
}
std::vector<GameMove> AIPlayerMinimax::GetAllMoves(BoardData::CellState playerTokenType, const BoardData& board) const
{
	std::vector<GameMove> moves;

	for (int y = 0; y < BoardData::BOARD_SIZE; y++)
		for (int x = 0; x < BoardData::BOARD_SIZE; x++)
		{
			GameMove m(x, y);

			if (board.IsCellEmpty(m) && !board.FindClosestNeighbours(m, playerTokenType).empty())
				moves.push_back(m);
		}

	return moves;
}
int AIPlayerMinimax::CalcScore(const BoardData& board, const Node& node, BoardData::CellState currentToken) const
{
	//Score is the difference between current-time our tokens and future-time our tokens
	//If win is possible the score is higher than any possible gain
	auto playerToken = currentToken;
	if (node.type == false)
		playerToken = BoardData::ReverseToken(currentToken);

	int pastScore = board.CountTokens(playerToken);
	int futureScore = node.board.CountTokens(playerToken);

	if (GetAllMoves(currentToken, node.board).empty())
	{	//turnig player cant turn

		auto opponentToken = BoardData::ReverseToken(playerToken);
		if (node.board.CountTokens(playerToken) > node.board.CountTokens(opponentToken))
		{ //VICTORY
			return 1000;
		}
		if (node.board.CountTokens(playerToken) < node.board.CountTokens(opponentToken))
		{ //DEFEAT
			return -1000;
		}
	}

	return futureScore - pastScore;
}
