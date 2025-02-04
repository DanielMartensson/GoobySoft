/*
 * modbus.h
 *
 *  Created on: Dec 3, 2024
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_TOOLS_SOFTWARE_LIBRARIES_MODBUS_MODBUS_H_
#define SRC_TOOLS_SOFTWARE_LIBRARIES_MODBUS_MODBUS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* For all */
void modbus_set_serial_write(int32_t (*serial_write)(const char port[], const uint8_t*, uint16_t, int32_t));
void modbus_set_serial_read(int32_t (*serial_read)(const char port[], uint8_t*, uint16_t, int32_t));
void modbus_set_serial_port(const char port[]);

/* Server functions */
bool modbus_server_create_RTU(const uint8_t address);
bool modbus_server_polling();
bool modbus_server_set_digital_outputs(const uint8_t outputs[], const uint16_t address, const uint16_t quantity);
bool modbus_server_set_digital_inputs(const uint8_t inputs[], const uint16_t address, const uint16_t quantity);
bool modbus_server_set_analog_inputs(const uint16_t inputs[], const uint16_t address, const uint16_t quantity);
bool modbus_server_get_parameters(uint16_t parameters[], const uint16_t address, const uint16_t quantity);
bool modbus_server_set_parameters(const uint16_t parameters[], const uint16_t address, const uint16_t quantity);

/* Client functions */
bool modbus_client_create_RTU(const uint8_t address);
void modbus_client_set_RTU_address(const uint8_t address);
bool modbus_client_get_digital_outputs(uint8_t outputs[], const uint16_t address, const uint16_t quantity);
bool modbus_client_get_digital_inputs(uint8_t inputs[], const uint16_t address, const uint16_t quantity);
bool modbus_client_get_analog_inputs(uint16_t inputs[], const uint16_t address, const uint16_t quantity);
bool modbus_client_set_parameters(const uint16_t parameters[], const uint16_t address, const uint16_t quantity);
bool modbus_client_get_parameters(uint16_t parameters[], const uint16_t address, const uint16_t quantity);

#ifdef __cplusplus
}
#endif

#endif /* SRC_TOOLS_SOFTWARE_LIBRARIES_MODBUS_MODBUS_H_ */
