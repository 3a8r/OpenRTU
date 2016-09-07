#ifndef OPENRTU_SRC_MODBUS_H_
#define OPENRTU_SRC_MODBUS_H_

#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus-version.h>
#include <modbus/modbus.h>
#include <vector>
#include <iostream>
#include <memory>
#include <thread>
#include <string>

using namespace std;

#define SERVER_ID         17
#define INVALID_SERVER_ID 18

const uint16_t UT_BITS_ADDRESS = 0x130;
const uint16_t UT_BITS_NB = 0x25;
const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };

const uint16_t UT_INPUT_BITS_ADDRESS = 0x1C4;
const uint16_t UT_INPUT_BITS_NB = 0x16;
const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };

const uint16_t UT_REGISTERS_ADDRESS = 0x160;
const uint16_t UT_REGISTERS_NB = 0x3;
const uint16_t UT_REGISTERS_NB_MAX = 0x20;
const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };

/* Raise a manual exception when this address is used for the first byte */
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x170;
/* The response of the server will contains an invalid TID or slave */
const uint16_t UT_REGISTERS_ADDRESS_INVALID_TID_OR_SLAVE = 0x171;
/* The server will wait for 1 second before replying to test timeout */
const uint16_t UT_REGISTERS_ADDRESS_SLEEP_500_MS = 0x172;
/* The server will wait for 5 ms before sending each byte */
const uint16_t UT_REGISTERS_ADDRESS_BYTE_SLEEP_5_MS = 0x173;

/* If the following value is used, a bad response is sent.
   It's better to test with a lower value than
   UT_REGISTERS_NB_POINTS to try to raise a segfault. */
const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;

const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x108;
const uint16_t UT_INPUT_REGISTERS_NB = 0x1;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };

const float UT_REAL = 123456.00;

const uint32_t UT_IREAL_ABCD = 0x0020F147;
const uint32_t UT_IREAL_DCBA = 0x47F12000;
const uint32_t UT_IREAL_BADC = 0x200047F1;
const uint32_t UT_IREAL_CDAB = 0xF1470020;

class ModbusServerGroupData
{
public:

	ModbusServerGroupData(uint16_t _ID,uint16_t _ModbusServerGroupID,
						  uint16_t _Number,int16_t  _Multiplier,
						  uint16_t _Type)
	{
		ID = _ID;
		ModbusServerGroupID = _ModbusServerGroupID;
		Number = _Number;
		Multiplier = _Multiplier;
		Type = _Type;
	}
public:

	uint16_t ID;
	uint16_t ModbusServerGroupID;
	uint16_t Number;
	int16_t  Multiplier;
	uint16_t Type;
	std::vector<std::shared_ptr<int>> Bits;
};

class ModbusServerGroup
{
public:
	ModbusServerGroup(uint16_t _ID,uint16_t _ModbusServerID,
					  std::string _Name,uint16_t _Start,
					  uint16_t _Len,uint16_t _Function,
					  uint16_t _CycleTime)
	{
		ID = _ID;
		ModbusServerID = _ModbusServerID;
		Name = _Name;
		Start = _Start;
		Len = _Len;
		Function = _Function;
		CycleTime = _CycleTime;
	}
public:
	uint16_t ID;
	uint16_t ModbusServerID;
	std::string Name;
	uint16_t Start;
	uint16_t Len;
	uint16_t Function;
	uint16_t CycleTime;
	std::vector<std::shared_ptr<ModbusServerGroupData>> ModbusGroupData;
};


class ModbusServer
{
public:
	enum ModbusType
	{
		TCP = 0,
		RTU = 1
	};

    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;

	ModbusServer(uint16_t _ID,uint16_t _ModbusType,uint16_t _Port,
				 std::string _IP,uint16_t _StopBit,uint16_t _DataSize,
				 uint16_t _Parity,uint16_t _Baudrate,std::string _SerialPortName,
				 std::string _Name,uint16_t _SlaveNumber,uint16_t _DiagnosticID,
				 uint16_t _ProtocolAddress)
	{
		ID = _ID;
		ModbusType = _ModbusType;
		Port = _Port;
		IP = _IP;
		StopBit = _StopBit;
		DataSize = _DataSize;
		Parity = _Parity;
		Baudrate = _Baudrate;
		SerialPortName = _SerialPortName;
		Name = _Name;
		SlaveNumber = _SlaveNumber;
		DiagnosticID = _DiagnosticID;
		ProtocolAddress = _ProtocolAddress;
		RunThreadExit = false;

	}

	bool RunThreadExit;
	std::thread RunThreadID;
	void StartServer();
	void ProcessModbus();
	virtual ~ModbusServer();

public:
	uint16_t ID;
	uint16_t ModbusType;
	uint16_t ServerID;
	uint16_t Port;
	std::string IP;
	uint16_t StopBit;
	uint16_t DataSize;
	uint16_t Parity;
	uint16_t Baudrate;
	std::string SerialPortName;
	std::string Name;
	uint16_t SlaveNumber;
	uint16_t DiagnosticID;
	uint16_t ProtocolAddress;
	std::vector<std::shared_ptr<ModbusServerGroup>> ModbusGroup;
};
#endif /* OPENRTU_SRC_MODBUS_H_ */
