/*
 * ModbusServer.h
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus-version.h>
#include <modbus/modbus.h>
#include <vector>
#include <string>
using namespace std;

#ifndef OPENRTU_SRC_MODBUS_H_
#define OPENRTU_SRC_MODBUS_H_
class ModbusServerGroupData
{
public:
	ModbusServerGroupData(uint16_t _ID,uint16_t _ModbusServerGroupID,
						  uint16_t _Number,int16_t  _Multiplier,
						  uint16_t _Type,vector<int> _Bits)
	{
		ID = _ID;
		ModbusServerGroupID = _ModbusServerGroupID;
		Number = _Number;
		Multiplier = _Multiplier;
		Type = _Type;
		Bits = _Bits;
	}
public:

	uint16_t ID;
	uint16_t ModbusServerGroupID;
	uint16_t Number;
	int16_t  Multiplier;
	uint16_t Type;
	vector<int> Bits;
};

class ModbusServerGroup
{
public:
	ModbusServerGroup(uint16_t _ID,uint16_t _ModbusServerID,
					  std::string _Name,uint16_t _Start,
					  uint16_t _Len,uint16_t _Function,
					  uint16_t _CycleTime,vector<ModbusServerGroupData> _ModbusGroupData)
	{
		ID = _ID;
		ModbusServerID = _ModbusServerID;
		Name = _Name;
		Start = _Start;
		Len = _Len;
		Function = _Function;
		CycleTime = _CycleTime;
		ModbusGroupData = _ModbusGroupData;
	}
public:
	uint16_t ID;
	uint16_t ModbusServerID;
	std::string Name;
	uint16_t Start;
	uint16_t Len;
	uint16_t Function;
	uint16_t CycleTime;
	vector<ModbusServerGroupData> ModbusGroupData;
};


class ModbusServer
{
public:
	ModbusServer(uint16_t _ID,uint16_t _ModbusType,uint16_t _Port,
				 std::string _IP,uint16_t _StopBit,uint16_t _DataSize,
				 uint16_t _Parity,uint16_t _Baudrate,std::string _SerialPortName,
				 std::string _Name,uint16_t _SlaveNumber,uint16_t _DiagnosticID,
				 uint16_t _ProtocolAddress,std::vector<ModbusServerGroup> _ModbusGroup)
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
		ModbusGroup = _ModbusGroup;

	}
	virtual ~ModbusServer();
public:
	uint16_t ID;
	uint16_t ModbusType;
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
	std::vector<ModbusServerGroup> ModbusGroup;
};
#endif /* OPENRTU_SRC_MODBUS_H_ */
