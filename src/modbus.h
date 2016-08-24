/*
 * modbus.h
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus-version.h>
#include <modbus/modbus.h>

#ifndef OPENRTU_SRC_MODBUS_H_
#define OPENRTU_SRC_MODBUS_H_

class modbus
{
public:
	modbus();
	virtual ~modbus();
};
class modbus_client
{
public:
	modbus_client();
};

class modbus_server
{

public:
	modbus_server();
};

#endif /* OPENRTU_SRC_MODBUS_H_ */
