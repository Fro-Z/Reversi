#include "PacketBoardData.h"
PacketBoardData::PacketBoardData():board()
{

}
PacketBoardData::PacketBoardData(const BoardData& board):board(board)
{
	static_assert(((BoardData::BOARD_SIZE*BoardData::BOARD_SIZE + 1)<=MAX_PACKET_LENGTH),
			"Packet::MAX_PACKET_LENGTH is set too low. ");
}
PacketBoardData::~PacketBoardData()
{

}
int PacketBoardData::WriteToBuffer(char* buffer, int length) const
{
	int requiredSize = BoardData::BOARD_SIZE*BoardData::BOARD_SIZE + 1;
	if(length<requiredSize)
		throw PacketSmallBufferException();

	int writePos = 0;
	//write packet ID
	buffer[writePos++]=static_cast<char>(Packet::PacketType::PACKET_BOARD_DATA);

	//write board data
	for(int y=0;y<BoardData::BOARD_SIZE;y++)
		for(int x=0;x<BoardData::BOARD_SIZE;x++)
		{
			buffer[writePos++]=static_cast<char>(board.At(y,x));
		}

	return writePos; //num of bytes written

}
int PacketBoardData::ReadFromBuffer(char* buffer, int length)
{
	int requiredSize = BoardData::BOARD_SIZE*BoardData::BOARD_SIZE + 1;
	if(length<requiredSize)
		throw PacketSmallBufferException();

	int readPos = 0;
	//read packet ID
	if(buffer[readPos++]!=Packet::PacketType::PACKET_BOARD_DATA)
		throw PacketRestoreException();

	//read board data
	for(int y=0;y<BoardData::BOARD_SIZE;y++)
		for(int x=0;x<BoardData::BOARD_SIZE;x++)
		{
			board.At(y,x)=static_cast<BoardData::CellState>(buffer[readPos++]);
		}

	return readPos; //number of bytes read
}
BoardData& PacketBoardData::GetBoard()
{
	return board;
}
