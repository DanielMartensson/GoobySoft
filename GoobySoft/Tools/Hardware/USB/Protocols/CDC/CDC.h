#ifndef CDC
#define CDC
#include <string>
#include <vector>

/*
 * CDC stands for Communications Device Class
 */

bool Tools_Hardware_USB_Protocols_CDC_closeConnection(const char port[]);
bool Tools_Hardware_USB_Protocols_CDC_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const unsigned int flowControl, const unsigned int stopBits, const unsigned int parity);
std::vector<std::string> Tools_Hardware_USB_Protocols_CDC_getAllPorts();
bool Tools_Hardware_USB_Protocols_CDC_isConnected(const char port[]);
std::string Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();
int32_t Tools_Hardware_USB_Protocols_CDC_write(const char port[], const uint8_t data[], const uint16_t size, const int32_t timeout_ms);
int32_t Tools_Hardware_USB_Protocols_CDC_read(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms);
std::vector<uint8_t> Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(const char port[], const long long timeoutMilliseconds, uint8_t dataTX[], size_t size, std::string endingOfDataRX = "\r\n");

#endif // !CDC
