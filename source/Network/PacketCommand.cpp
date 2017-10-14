#include "PacketCommand.h"

PacketCommand::PacketCommand():command(COMMAND_NOTHING)
{

}
PacketCommand::PacketCommand(Commands command):command(command)

{

}
PacketCommand::~PacketCommand()
{
}
int PacketCommand::WriteToBuffer(char* buffer, int length) const
{
	if(length<1)
		throw PacketSmallBufferException();

	int writePos=0;
	buffer[writePos++]=Packet::PACKET_COMMAND;

	buffer[writePos++]=static_cast<char>(command);
	return writePos;
}
int PacketCommand::ReadFromBuffer(char* buffer, int length)
{
	if(length<1)
		throw PacketSmallBufferException();

	int readPos=0;
	if(buffer[readPos++]!=Packet::PACKET_COMMAND)
		throw PacketRestoreException();

	command = static_cast<Commands>(buffer[readPos++]);

	return readPos;
}

PacketCommand::Commands PacketCommand::GetCommand() const
{
	return command;
}
