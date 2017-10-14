#ifndef PACKETNEXTTURN_H_
#define PACKETNEXTTURN_H_
#include "Packet.h"
///Packet containing simple commands
class PacketCommand:public Packet
{
public:
	PacketCommand();
	enum Commands
		{
			COMMAND_NOTHING = 0,
			COMMAND_NEXT_TURN=1,
			COMMAND_REQUEST_MOVE=2,
			COMMAND_MOVE_REFUSED=3,
			COMMAND_PING=4
		};

	PacketCommand(Commands command);
	virtual ~PacketCommand();

	///Writes packet data to buffer, including packet ID
	virtual int WriteToBuffer(char* buffer, int length) const override;


	///Reads packet data from buffer, including packet ID
	virtual int ReadFromBuffer(char* buffer, int length) override;

	Commands GetCommand() const;
	void SetCommand(Commands newCommand);
private:
	Commands command;

};



#endif /* PACKETNEXTTURN_H_ */
