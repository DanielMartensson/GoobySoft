#ifndef Modbus
#define Modbus
#include <string>
#include <vector>

bool Tools_Hardware_USB_Protocols_Modbus_closeConnection(const char port[]);
bool Tools_Hardware_USB_Protocols_Modbus_readRegister(const char port[], const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]);
bool Tools_Hardware_USB_Protocols_Modbus_openConnection(const char port[], const unsigned int baudrate, const char parity, const unsigned int dataBit, const unsigned int stopBits);
bool Tools_Hardware_USB_Protocols_Modbus_isConnected(const char port[]);
bool Tools_Hardware_USB_Protocols_Modbus_setSlaveAddress(const char port[], const unsigned int slaveAddress);
std::vector<std::string> Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices();
#endif // !Modbus
