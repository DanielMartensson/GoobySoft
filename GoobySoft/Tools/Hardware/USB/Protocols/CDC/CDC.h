#ifndef CDC
#define CDC
#include <string>
#include <vector>

bool Tools_Hardware_USB_Protocols_CDC_closeConnection(const char port[]);
bool Tools_Hardware_USB_Protocols_CDC_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const unsigned int flowControl, const unsigned int stopBits, const unsigned int parity);
std::vector<std::string> Tools_Hardware_USB_Protocols_CDC_getAllPorts();
bool Tools_Hardware_USB_Protocols_CDC_isConnected(const char port[]);
std::string Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();
std::vector<uint8_t> Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(const char port[], const long long timeoutMilliseconds, uint8_t dataTX[], size_t size, std::string endingOfDataRX = "\r\n");

#endif // !CDC
