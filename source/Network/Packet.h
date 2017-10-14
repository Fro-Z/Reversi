#ifndef PACKET_H_
#define PACKET_H_
class PacketException /// Packet exception base
{};
class PacketRestoreException:public PacketException /// Packet could not be restored
{};
class PacketSmallBufferException: public PacketException /// Buffer to read or write Packet is too small
{};

/**
 * Packet base class
 */
class Packet
{
public:
	Packet();
	virtual ~Packet();

	///Writes packet data to buffer, including packet ID
	/// @return bytes written
	virtual int WriteToBuffer(char* buffer, int length) const = 0;

	///Reads packet data from buffer
	/// @return bytes read
	virtual int ReadFromBuffer(char* buffer, int length) = 0;

	///Type of packet used to form packet from raw data
	enum PacketType
	{
		PACKET_UNDEFINED = 0,
		PACKET_BOARD_DATA = 1,
		PACKET_GAME_MOVE = 2,
		PACKET_COMMAND = 3
	};
	static const int MAX_PACKET_LENGTH = 500;

private:

};

#endif /* PACKET_H_ */
