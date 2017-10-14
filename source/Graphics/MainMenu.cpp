#include "MainMenu.h"
#include "../GameEngine.h"
#include "TextWindow.h"

MainMenu::MainMenu():selection(0),w_startGame(nullptr),w_joinGameIP(nullptr),playerType1(PLAYER_LOCAL),playerType2(PLAYER_LOCAL)
{
	w_startGame = new TextWindow("Start game",(TERM_WIDTH-20)/2,1/2,20,5);
	windows.emplace_back(unique_ptr<Window>(w_startGame));

	w_player1 = new TextWindow("Player 1",(TERM_WIDTH-20)/2,5,20,3);
	windows.emplace_back(unique_ptr<Window>(w_player1));

	w_player2=new TextWindow("Player 2",(TERM_WIDTH-20)/2,9,20,3);
	windows.emplace_back(unique_ptr<Window>(w_player2));

	w_joinGame=new TextWindow("Join game:",(TERM_WIDTH-20)/2,13,20,5);
	windows.emplace_back(unique_ptr<Window>(w_joinGame));

	w_joinGameIP= new InputWindow((TERM_WIDTH-20)/2,19,20);
	windows.emplace_back(unique_ptr<Window>(w_joinGameIP));

	for(auto& winptr: windows)
	{
		winptr->SetParent(*this);
	}

	UpdateBorder();
	UpdateText();

	GameEngine::GetInstance().AddKeyboardListener(this);
}
MainMenu::~MainMenu()
{
	GameEngine::GetInstance().RemoveKeyboardListener(this);
}
void MainMenu::Draw()
{
	for(auto& winptr: windows)
		winptr->Draw();

	SetDirty(false);
}
void MainMenu::OnKeyPress(int key)
{
	if(selection==SELECT_JOINGAME_IP && (key==KEY_UP || key ==KEY_DOWN))
		GameEngine::GetInstance().RemoveKeyboardListener(w_joinGameIP); //scrolling out of text input window

	if(key==KEY_DOWN) //scroll down
	{
		selection++;
		if(selection>SELECT_JOINGAME_IP)
			selection=SELECT_STARTGAME;
	}
	if(key==KEY_UP) //scroll up
	{
		selection--;
		if(selection<SELECT_STARTGAME)
			selection=SELECT_JOINGAME_IP;
	}
	if(key==KEY_LEFT)
	{
		if(selection==SELECT_PLAYER1)
		{
			if(--playerType1 < PLAYER_LOCAL)
				playerType1=PLAYER_AI_MINIMAX; //player1 as remote player is not allowed (yet)
		}
		if(selection==SELECT_PLAYER2)
		{
			if(--playerType2 < PLAYER_LOCAL)
				playerType2=PLAYER_REMOTE;
		}
		UpdateText();
	}
	if(key==KEY_RIGHT)
	{
		if(selection==SELECT_PLAYER1)
		{
			if(++playerType1 > PLAYER_AI_MINIMAX)
				playerType1=PLAYER_LOCAL; //player1 as remote player is not allowed (yet)
		}
		if(selection==SELECT_PLAYER2)
		{
			if(++playerType2 > PLAYER_REMOTE)
				playerType2=PLAYER_LOCAL;
		}
		UpdateText();
	}
	if(key==KEY_ENTER || key=='\n')
	{
		if(selection==SELECT_STARTGAME)
		{
			GameEngine::GetInstance().StartGame(static_cast<PlayerType>(playerType1),static_cast<PlayerType>(playerType2));
			return; //menu will get deleted, stop execution of menu functions
		}
		if(selection==SELECT_JOINGAME)
		{
			GameEngine::GetInstance().JoinGame(static_cast<PlayerType>(playerType1),w_joinGameIP->GetText());
			return; //menu will get deleted, stop execution of menu functions
		}
	}

	if(selection==SELECT_JOINGAME_IP && (key==KEY_UP || key ==KEY_DOWN)) //selected the input win
	{
		GameEngine::GetInstance().AddKeyboardListener(w_joinGameIP);
	}

	UpdateBorder();
	SetDirty(true);
}
void MainMenu::UpdateBorder()
{
	for(auto& winptr:windows)
	{
		winptr->SetBorder(false);
	}

	windows.at(selection)->SetBorder(true);

	SetDirty(true);
}
void MainMenu::UpdateText()
{
	w_player1->SetText(PlayerTypes::GetPlayerTypeDescription(static_cast<PlayerType>(playerType1)).c_str());
	w_player2->SetText(PlayerTypes::GetPlayerTypeDescription(static_cast<PlayerType>(playerType2)).c_str());
}
