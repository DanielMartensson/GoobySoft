#ifndef ModbusRTU
#define ModbusRTU
#include <string>

bool Tools_Hardware_USB_Protocols_ModbusRTU_closeConnection(const char port[]);
bool Tools_Hardware_USB_Protocols_ModbusRTU_readRegister(const char port[], const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]);
bool Tools_Hardware_USB_Protocols_ModbusRTU_openConnection(const char port[], const unsigned int baudrate, const char parity, const unsigned int dataBit, const unsigned int stopBits);
bool Tools_Hardware_USB_Protocols_ModbusRTU_openConnection(const char ipAddress[], const unsigned int port);
bool Tools_Hardware_USB_Protocols_ModbusRTU_isConnected(const char port[]);
bool Tools_Hardware_USB_Protocols_ModbusRTU_setSlaveAddress(const char port[], const unsigned int slaveAddress);
std::string Tools_Hardware_USB_Protocols_ModbusRTU_getPortsOfConnectedDevices(bool isTCP);

#endif // !ModbusRTU
