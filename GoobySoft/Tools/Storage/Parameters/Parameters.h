#ifndef Parameters
#define Parameters
#include "../../Hardware/USB/USB.h"

struct USBSettings {
	unsigned int baudrate = 9600;
	unsigned int dataBit = 8;
	int parityIndex = 0;
	int stopBitIndex = 0;
	int controlFlowIndex = 0;
	int protocolIndex = 0;
	char port[256] = { 0 };
};

struct ModbusSettings {
	int deviceAddress = 1;
};

struct FileSettings {
	char filePathName[1024] = "";
	char folderPath[1024] = "";
	char fileName[256] = "";
};

struct DatabaseSettings {
	char host[50] = "127.0.0.1";
	char schema[50] = "schema";
	int addDevice = 33060;
	char username[50] = "myUser";
	char password[50] = "myPassword";
};

typedef struct {
	USBSettings usbSettings[MAX_USB_DEVICES];
	ModbusSettings modbusSettings[MAX_USB_DEVICES];
	DatabaseSettings databaseSettings;
	FileSettings fileSettings;
}ParameterHolder;

void Tools_Hardware_ParameterStore_loadParameters();
void Tools_Hardware_ParameterStore_saveParameters();
ParameterHolder* Tools_Hardware_ParameterStore_getParameterHolder();

#endif // !Parameters