#include <ncurses.h>
#include <iostream>
#include <memory>

#include "GameEngine.h"
#include "Graphics/TextWindow.h"
#include "Graphics/GameBoard.h"
#include "Graphics/MoveInputWindow.h"
#include "AIPlayerSimple.h"
#include "AIPlayerMinimax.h"
#include "LocalPlayer.h"
#include "RemotePlayer.h"
#include "RemoteGameMatch.h"
#include "Graphics/MainMenu.h"

using std::make_unique;
using std::unique_ptr;

GameEngine::GameEngine() :
		currentState(STATE_INIT), currentMatch(nullptr), boardDisplay(nullptr),
		turnInfoBox(nullptr), inputWin(nullptr), popupWin(nullptr), breakKL(false)
{
	void ShowMainMenu();
}
GameEngine::~GameEngine()
{
	CleanupGame();
	endwin(); //end curses mode
}
void GameEngine::DeletePlayers()
{
	players.clear();
}
void GameEngine::Initialize()
{
	if (currentState != STATE_INIT)
		return;

	initscr();	//start curses mode

	if (has_colors())
		start_color();

	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_RED, COLOR_GREEN);
	attron(COLOR_PAIR(0));

	curs_set(0);
	clear();
	cbreak();
	keypad(stdscr, TRUE);	//enable F1-F12 keys
	nodelay(stdscr, TRUE);
	noecho();
	refresh();

	ShowMainMenu();
}
void GameEngine::Think()
{
	HandleKeyPress();

	if (currentMatch && currentState == STATE_GAME)
		currentMatch->Think();

	if (currentMatch && currentState == STATE_GAME)
	{
		if (currentMatch->HasEnded())
			DisplayPostGameScreen();

		for (auto& player : players)
			if (player->IsRemote())
				dynamic_cast<RemotePlayer*>(player.get())->Think();
	}

	Draw();
}
void GameEngine::DisplayPostGameScreen()
{
	currentState = STATE_POSTGAME;
	std::string text;
	if (currentMatch)
		text = currentMatch->GetGameInfo();

	if (turnInfoBox)
		UpdateTurnInfoBox();

	screenRoot.AddChild(std::make_unique<TextWindow>(text.c_str(), 45, 10, 25, 5));
}
void GameEngine::Draw()
{
	if (screenRoot.IsDirty()) //draw only when needed
	{
		//repaint all with whitespace to get rid of remains of deleted windows
		for (int y = 0; y < LINES; y++)
			for (int x = 0; x < COLS; x++)
				mvaddch(y, x, ' ');
		refresh();

		screenRoot.Draw();
	}
}
void GameEngine::AddKeyboardListener(KeyboardListener* listener)
{
	keyboardListeners.push_back(listener);
	breakKL = true; //avoid iterator invalidation when called from within HandleKeyPress loop
}
void GameEngine::RemoveKeyboardListener(KeyboardListener* listener)
{
	keyboardListeners.remove(listener);
	breakKL = true; //avoid iterator invalidation when called from within HandleKeyPress loop
}
void GameEngine::HandleKeyPress()
{
	int ch = getch();
	while (ch != ERR)
	{
		if (popupWin) //close popup win if exists
		{
			screenRoot.DeleteChild(popupWin);
			popupWin = nullptr;
			ch = getch();
			continue; //popup eats the key
		}

		/**
		 * If any changes to KeyboardListener list happen as a response to OnKeyPress event
		 * the list iterator will be invalidated. The loop should be broken in this case.
		 *
		 * Member functions manipulating with keyboardListener must set breakKL=true
		 */
		breakKL = false;

		for (auto listener : keyboardListeners) //pass key to listeners
		{
			if (listener)
				listener->OnKeyPress(ch);
			if (breakKL)
			{
				break;
			}
		}

		if (currentState == STATE_POSTGAME && (ch == KEY_ENTER || ch == 10))
		{
			CleanupGame();
			ShowMainMenu();
		}
		if (ch == KEY_TERMINATE) //exit to menu / desktop
		{
			CleanupGame();
			if (currentState == STATE_MENU)
				exit(0);
			else
				ShowMainMenu();
		}
		ch = getch();
	}
}
void GameEngine::CleanupGame()
{
	screenRoot.Clear();
	delete currentMatch;
	currentMatch = nullptr;

	//memory was freed by screen
	inputWin = nullptr;
	turnInfoBox = nullptr;
	boardDisplay = nullptr;
	popupWin = nullptr;

	DeletePlayers();
}
void GameEngine::ShowMainMenu()
{
	CleanupGame();

	currentState = STATE_MENU;
	screenRoot.AddChild(make_unique<MainMenu>());
}
void GameEngine::UpdateTurnInfoBox()
{
	std::string text;
	if (currentMatch->GetTurningPlayerToken() == BoardData::CELL_BLACK)
		text += "BLACK -";
	else
		text += "WHITE -";

	text += (currentMatch->GetTurningPlayer()->GetName());
	text += " turns";
	turnInfoBox->SetText(text.c_str());
}


void GameEngine::SetLocalPlayerInputWin(LocalPlayer* player)
{
	if (!inputWin)
	{
		inputWin = new MoveInputWindow(45, 6);
		screenRoot.AddChild(unique_ptr<MoveInputWindow>(inputWin));
	}

	player->SetMoveInputWin(inputWin);	
}

void GameEngine::OnStartTurn()
{
	if (boardDisplay)
		boardDisplay->UpdateBoard();

	if (turnInfoBox && currentMatch)
		UpdateTurnInfoBox();
}
void GameEngine::StartGame(PlayerType p1Type, PlayerType p2Type)
{
	CleanupGame();

	unique_ptr<Player> p1 = CreatePlayerByType(p1Type);
	unique_ptr<Player> p2;
	try
	{
		p2 = CreatePlayerByType(p2Type); //RemotePlayer may throw exception on unsuccessful connection
	}
	catch (ServerException& e)
	{
		ShowMainMenu();
		DisplayPopup("Could not start server");
		return;
	}

	if (p1Type == PLAYER_LOCAL)
		SetLocalPlayerInputWin(dynamic_cast<LocalPlayer*>(p1.get()));
	if (p2Type == PLAYER_LOCAL)
		SetLocalPlayerInputWin(dynamic_cast<LocalPlayer*>(p2.get()));


	currentMatch = new GameMatch(p1.get(), p2.get());
	players.emplace_back(std::move(p1));
	players.emplace_back(std::move(p2));

	BuildGameUI();
	currentState = STATE_GAME;
	HidePopup(); //Hide any remaining popup from menu
}

void GameEngine::BuildGameUI()
{
	boardDisplay = new GameBoard(0, 0, &currentMatch->GetBoard(), currentMatch);
	screenRoot.AddChild(unique_ptr<GameBoard>(boardDisplay));

	turnInfoBox = new TextWindow("", 45, 1, 34, 5);
	screenRoot.AddChild(unique_ptr<TextWindow>(turnInfoBox));
	UpdateTurnInfoBox();
}

void GameEngine::JoinGame(PlayerType p1Type, std::string address)
{
	CleanupGame();

	unique_ptr<Player> p1 = CreatePlayerByType(p1Type);

	if (p1Type == PLAYER_LOCAL) //local players need input window to play
		SetLocalPlayerInputWin(dynamic_cast<LocalPlayer*>(p1.get()));
	
	try
	{
		currentMatch = new RemoteGameMatch(p1.get(), address.c_str(), RemotePlayer::LISTEN_PORT);
	}
	catch (ClientException& e)
	{
		CleanupGame();
		ShowMainMenu();
		DisplayPopup("Could not join match");
		return;
	}

	players.emplace_back(std::move(p1));
	BuildGameUI();

	currentState = STATE_GAME;
	HidePopup();
}
unique_ptr<Player> GameEngine::CreatePlayerByType(PlayerType type) const
{
	switch (type)
	{
		case PLAYER_LOCAL:
			return make_unique<LocalPlayer>("Local player");
		case PLAYER_AI_SIMPLE:
			return make_unique<AIPlayerSimple>("AI_SIMPLE");
		case PLAYER_AI_MINIMAX:
			return make_unique<AIPlayerMinimax>("AI_MINIMAX");
		case PLAYER_REMOTE:
			return make_unique<RemotePlayer>("Remote player");
		default:
			std::cerr << "\nERROR: This player type is not yet implemented in engine";
			exit(1);
	}
}
void GameEngine::DisplayPopup(const char* text)
{
	HidePopup();

	popupWin = new TextWindow(text, 25, 5, 30, 10);
	screenRoot.AddChild(unique_ptr<Drawable>(popupWin));
}
void GameEngine::HidePopup()
{
	if (popupWin)
		screenRoot.DeleteChild(popupWin);
	popupWin = nullptr;
}
