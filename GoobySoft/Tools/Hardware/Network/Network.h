#ifndef Network
#define Network
#include "Protocols/Protocols.h"

typedef enum {
	NETWORK_STATUS_FAIL,
	NETWORK_STATUS_OK,
	NETWORK_STATUS_CONNECTED,
	NETWORK_STATUS_NOT_EXIST,
	NETWORK_STATUS_DISCONNECTED,
	NETWORK_STATUS_NOT_CONNECTED,
	NETWORK_STATUS_MISSING_DATA,
	NETWORK_STATUS_NO_PROTOCOL
}NETWORK_STATUS;

NETWORK_STATUS Tools_Hardware_Network_isConnected(const char ipAddress[], const unsigned int port, const std::string& protocols);
NETWORK_STATUS Tools_Hardware_Network_openConnection(const char ipAddress[], const unsigned int port, const std::string& protocols);
NETWORK_STATUS Tools_Hardware_Network_closeConnection(const char ipAddress[], const unsigned int port, const std::string& protocols);
std::string Tools_Hardware_Network_getConnectedIpAddresses(const std::string& protocols);

#endif // !Network
