#include "Network.h"
#include "../../Tools.h"

NETWORK_STATUS Tools_Hardware_Network_isConnected(const char ipAddress[], const unsigned int port) {
	return NETWORK_STATUS_FAIL;
}

NETWORK_STATUS Tools_Hardware_Network_openConnection(const char ipAddress[], const unsigned int port) {
	return NETWORK_STATUS_FAIL;
}

NETWORK_STATUS Tools_Hardware_Network_closeConnection(const char ipAddress[], const unsigned int port) {
	return NETWORK_STATUS_FAIL;
}

std::string Tools_Hardware_Network_getConnectedIpAddresses() {
	return "FAIL";
}