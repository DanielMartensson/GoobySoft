#ifndef NetworkProtocols
#define NetworkProtocols

#include "ModbusTCP/ModbusTCP.h"

static const std::string NETWORK_PROTOCOL_STRING[] = { "Modbus TCP" };
typedef enum {
	NETWORK_PROTOCOL_ENUM_MODBUS_TCP
}NETWORK_PROTOCOL_ENUM;

#endif // !NetworkProtocols
