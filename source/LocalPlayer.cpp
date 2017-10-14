#include "LocalPlayer.h"
#include "GameEngine.h"
#include <iostream>

LocalPlayer::LocalPlayer(const std::string& name):Player(false,name),inputWin(nullptr)
{

}
LocalPlayer::~LocalPlayer()
{

}
bool LocalPlayer::IsReady() const
{
	if(inputWin && inputWin->IsInputConfirmed())
		return true;
	else
		return false;
}
void LocalPlayer::RefuseMove()
{
	if(inputWin)
		inputWin->Reset();
}
void LocalPlayer::OnTurnBegin(const GameMatch& match)
{
	if(inputWin)
		inputWin->Reset();
}
GameMove LocalPlayer::GetMove() const
{
	if(inputWin)
		return inputWin->GetMove();
	else
	{
		std::cerr<<"\nERROR: LocalPlayer::GetMove called when not ready!";
		return GameMove();
	}
}
void LocalPlayer::SetMoveInputWin(MoveInputWindow* win)
{
	inputWin=win;
}
