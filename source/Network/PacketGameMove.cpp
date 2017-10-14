#include "PacketGameMove.h"
PacketGameMove::PacketGameMove():move()
{

}
PacketGameMove::PacketGameMove(const GameMove& move):move(move)
{

}
PacketGameMove::~PacketGameMove()
{

}
int PacketGameMove::WriteToBuffer(char* buffer, int length) const
{
	int requiredLength = 3; //2 dimensions for game move, one byte for packet type ID
	if(length<requiredLength)
		throw PacketSmallBufferException();

	int writePos=0;
	//write ID
	buffer[writePos++]=static_cast<char>(Packet::PacketType::PACKET_GAME_MOVE);

	//write data
	buffer[writePos++]=static_cast<char>(move.x);
	buffer[writePos++]=static_cast<char>(move.y);

	return writePos; //num of bytes written
}
int PacketGameMove::ReadFromBuffer(char* buffer, int length)
{
	int requiredLength = 3; //2 dimensions for game move, one byte for packet type ID
	if(length<requiredLength)
		throw PacketSmallBufferException();

	int readPos=0;

	//verify ID
	if(buffer[readPos++]!=Packet::PacketType::PACKET_GAME_MOVE)
		throw PacketRestoreException();

	//restore data
	move.x=buffer[readPos++];
	move.y=buffer[readPos++];

	return readPos; //num of bytes read
}
GameMove& PacketGameMove::GetMove()
{
	return move;
}
