#ifndef ModbusTCP
#define ModbusTCP
#include <string>

bool Tools_Hardware_Network_Protocols_ModbusTCP_isConnected(const char ipAddress[], const unsigned int port);
bool Tools_Hardware_Network_Protocols_ModbusTCP_readRegister(const char ipAddress[], const unsigned int port, const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]);
bool Tools_Hardware_Network_Protocols_ModbusTCP_closeConnection(const char ipAddress[], const unsigned int port);
bool Tools_Hardware_Network_Protocols_ModbusTCP_openConnection(const char ipAddress[], const unsigned int port);
bool Tools_Hardware_Network_Protocols_ModbusTCP_setSlaveAddress(const char ipAddress[], const unsigned int port, const unsigned int slaveAddress);
std::string Tools_Hardware_Network_Protocols_ModbusTCP_getPortsOfConnectedDevices();

#endif // !ModbusTCP

