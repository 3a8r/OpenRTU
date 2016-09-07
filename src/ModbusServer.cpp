/*
 * modbus.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */

#include "ModbusServer.h"
#include <thread>
#include <unistd.h>
#include <functional>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <modbus/modbus-version.h>

using namespace std;

void ModbusServer::ProcessModbus()
{
	uint8_t *query;
    int header_length;

	if(ModbusType == TCP )
	{
		ctx = modbus_new_tcp(IP.c_str(), Port);
		query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
	}
	else if(ModbusType == RTU)
	{
		ctx = modbus_new_rtu(SerialPortName.c_str(), Baudrate, Parity, DataSize, StopBit);
		query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
		modbus_set_slave(ctx, ServerID);
	}

    mb_mapping = modbus_mapping_new_start_address(UT_BITS_ADDRESS, UT_BITS_NB,
												  UT_INPUT_BITS_ADDRESS, UT_INPUT_BITS_NB,
												  UT_REGISTERS_ADDRESS, UT_REGISTERS_NB_MAX,
												  UT_INPUT_REGISTERS_ADDRESS, UT_INPUT_REGISTERS_NB);

	header_length = modbus_get_header_length(ctx);

	while(!RunThreadExit)
	{

	}
}

void ModbusServer::StartServer()
{
	RunThreadID = std::thread(&ModbusServer::ProcessModbus,this);
}

ModbusServer::~ModbusServer()
{
	modbus_free(ctx);
}


