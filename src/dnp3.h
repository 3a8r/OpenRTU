#ifndef SRC_OBJECTS_dnp3_H_
#define SRC_OBJECTS_dnp3_H_

#include <thread>
#include <iostream>
#include <mutex>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/MeasUpdate.h>
#include <asiopal/UTCTimeSource.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/LogLevels.h>
//#include "dnp3/ace6000.h"
//#include "object_factory.h"

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;


class dnp3data
{
public:
	dnp3data(uint32_t _DNP3DataID,uint32_t _DNP3ID,
			 uint32_t _VarNameID,string	 _Name,
			 uint32_t _Index)
	{
		DNP3DataID = _DNP3DataID;
		DNP3ID = _DNP3ID;
		VarNameID = _VarNameID;
		Name = _Name;
		Index = _Index;
	};

	uint32_t DNP3DataID;
	uint32_t DNP3ID;
	uint32_t VarNameID;
	string	 Name;
	uint32_t Index;

};

class dnp3
{
public:
	uint32_t DNP3ID;
	uint32_t MeterID;
	string	 PortName;
	uint32_t DataID;
	uint32_t Interval;
	string   DNP3_OUTSTATION_Name;
	uint32_t DNP3_Port;
	uint32_t NoOfObject;
	bool	 AllowUnsolicited;
	uint32_t LocalAddr;
	uint32_t RemoteAddr;

	vector<dnp3data*> DNP3DataList;

	dnp3(uint32_t _DNP3ID,uint32_t _MeterID,
		  string _PortName,uint32_t _DataID,
		  uint32_t _Interval,string   _DNP3_OUTSTATION_Name,
		  uint32_t _DNP3_Port,uint32_t _NoOfObject,
		  bool _AllowUnsolicited,uint32_t _LocalAddr,
		  uint32_t _RemoteAddr)
	{
		DNP3ID = _DNP3ID;
		MeterID = _MeterID;
		PortName = _PortName;
		DataID = _DataID;
		Interval = _Interval;
		DNP3_OUTSTATION_Name = _DNP3_OUTSTATION_Name;
		DNP3_Port = _DNP3_Port;
		NoOfObject = _NoOfObject;
		AllowUnsolicited = _AllowUnsolicited;
		LocalAddr = _LocalAddr;
		RemoteAddr = _RemoteAddr;
	};
	static mutex objects_lock;
	static map<string, int> objects;

    std::shared_ptr<asiodnp3::IChannel> channel;
    std::shared_ptr<opendnp3::OutstationStackConfig> stackConfig; //TODO: options for creating stackConfig
    asiodnp3::IOutstation *outstation;

    std::shared_ptr<IChannel> create_dnp3_channel(std::string name, int port);
    void ConfigureDatabase(DatabaseConfigView view);
    int set_value(std::string name, double value) throw();
    std::list<std::string> get_objects(void);

	void Initialize(void);
	void init_objects(void);
	virtual ~dnp3();
};


#endif
