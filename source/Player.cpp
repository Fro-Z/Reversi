#include "Player.h"

Player::~Player()
{

}

Player::Player(bool remote,const std::string& name):remote(remote),name(name)
{

}
const std::string& Player::GetName() const
{
	return name;
}
bool Player::IsReady() const
{
	return false;
}
bool Player::IsRemote() const
{
	return remote;
}
