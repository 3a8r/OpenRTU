/*
 * rtdb.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */

#include "Rtdb.h"

#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <memory>
#include "dnp3.h"
#include "database.h"
#include "ModbusServer.h"
using namespace std;

Rtdb* Rtdb::instance = NULL;


Rtdb* Rtdb::Instance(){

	if(instance == NULL){

		instance = new Rtdb();
	}

	return instance;
}

Rtdb::Rtdb() {
	ID = 0;
	Name = "";
	Description = "";
	Revision = 0;
}
bool Rtdb::LoadConfigFile(){

	int rows = 0;

	std::string fulldbfilepath = ProjectPath + ConfigFileName;

	printf("\n File:%s",fulldbfilepath.c_str());

	if( ::access(fulldbfilepath.c_str(),F_OK) == -1 )
	{
		printf("\n Can't Find The Specified Config File.\n");
		char i = 0;
		while(::access(fulldbfilepath.c_str(),F_OK) == -1)
		{
			printf("Waiting For Config File ...%c\r",(i==0)?'|':(i==1)?'/':(i==2)?'-':'\\');
			fflush(stdout);
			if(++i == 4) i=0;
			usleep(500000);
		}
		printf("\b\n");
	}

	int fd;
	int filesize;

	if((fd = ::open(fulldbfilepath.c_str(),O_RDONLY)) != -1)
	{
		struct ::stat fst;
		unsigned char buffer[100];
		do{
			::fstat(fd,&fst);
			printf("\nFile mode = %d file size = %ld ",fst.st_mode,fst.st_size);
			::lseek(fd,0,SEEK_SET);
			::read(fd,buffer,sizeof(buffer));
			int pagesize = (buffer[16]<<8) | (buffer[17]<<16);
			unsigned int numberofpages = buffer[31] + (buffer[30] <<8) + (buffer[29] << 16) + (buffer [28] << 24);
			filesize = pagesize * numberofpages;
			usleep(100000);
		}
		while(filesize != fst.st_size);
		::close(fd);
	}



	database DB(fulldbfilepath);


	/*------------------------ Read RTU Config Table from sqlite file -----------------*/

	std::string tblname = "RTUConfig";
	vector<vector<string> > tbl = DB.GetTableValues(tblname);
	map<string,int> tblinfo = DB.GetTableFields(tblname);
	istringstream( tbl[0][tblinfo["ID"] ] ) >> ID;
	istringstream( tbl[0][tblinfo["Name"] ] ) >> Name;
	istringstream( tbl[0][tblinfo["Description"] ] ) >> Description;
	istringstream( tbl[0][tblinfo["Revision"] ] ) >> Revision;


	/*-------------------------Variable-----------------------------------------------*/


	//Filling Table Name
	tblname = "Variable";

	//Reading All Variables
	tblinfo.clear();
	tbl = DB.GetTableValues(tblname, NULL);
	tblinfo = DB.GetTableFields(tblname);

	uint32_t 	VarNameID;
	string	 	VarName;
	uint32_t	Type;
	string		Description;
	uint32_t	InitialValue;

	printf("\nReading All Variable Data ...");

	rows = tbl.size();


	for(int i=0;i<rows;i++)
	{
		istringstream( tbl[i][tblinfo["VarID"]]) >> VarNameID;
		istringstream( tbl[i][tblinfo["VarName"]]) >> VarName;
		istringstream( tbl[i][tblinfo["Type"]]) >> Type;
		istringstream( tbl[i][tblinfo["Description"]]) >> Description;
		istringstream( tbl[i][tblinfo["InitialValue"]]) >> InitialValue;

		VariableList.push_back(new variable(VarNameID,VarName,Type,Description,InitialValue));
	}


	/*-------------------------DNP3-----------------------------------------------*/

	//Filling Table Name
	tblname = "DNP3";
	//Reading All DlmsData
	tblinfo.clear();
	tbl = DB.GetTableValues(tblname, NULL);
	tblinfo = DB.GetTableFields(tblname);

	uint32_t DNP3ID;
	string	 PortName;
	uint32_t DataID;
	string   DNP3_OUTSTATION_Name;
	uint32_t DNP3_Port;
	uint32_t NoOfObject;
	bool	 AllowUnsolicited;
	uint32_t LocalAddr;
	uint32_t RemoteAddr;


	printf("\nReading DNP3 ...");

	rows = tbl.size();


	for(int i=0;i<rows;i++)
	{
		istringstream( tbl[i][tblinfo["ID"]]) >> DNP3ID;
		istringstream( tbl[i][tblinfo["PortName"]]) >> PortName;
		istringstream( tbl[i][tblinfo["DataID"]]) >> DataID;
		istringstream( tbl[i][tblinfo["OutstationName"]]) >> DNP3_OUTSTATION_Name;
		istringstream( tbl[i][tblinfo["PortNumber"]]) >> DNP3_Port;
		istringstream( tbl[i][tblinfo["NoOfObject"]]) >> NoOfObject;
		istringstream( tbl[i][tblinfo["AllowUnsolicited"]]) >> AllowUnsolicited;
		istringstream( tbl[i][tblinfo["LocalAddr"]]) >> LocalAddr;
		istringstream( tbl[i][tblinfo["RemoteAddr"]]) >> RemoteAddr;

		dnp3 _dnp3(DNP3ID,PortName,DataID,DNP3_OUTSTATION_Name,
				  DNP3_Port,NoOfObject,AllowUnsolicited,LocalAddr,RemoteAddr);


		/*-------------------------DNP3Data-----------------------------------------------*/


		string tblname = "DNP3Data";
		vector<vector<string> > sdtbl;
		map<string, int> sdtblinfo;
		multimap<string, string> clause;
		pair<string, string> p("DNP3ID",tbl[i][tblinfo["DNP3ID"]]);
		clause.insert(p);
		sdtbl = DB.GetTableValues(tblname,NULL,&clause);
		sdtblinfo = DB.GetTableFields(tblname);

		for(vector<vector<string> >::iterator row = sdtbl.begin(); row != sdtbl.end(); row++)
		{

			uint32_t DNP3DataID;
			uint32_t DNP3ID;
			uint32_t VarNameID;
			string	 Name;
			uint32_t Index;

			istringstream( (*row)[sdtblinfo["ID"]]) >> DNP3DataID;
			istringstream( (*row)[sdtblinfo["DNP3ID"]]) >> DNP3ID;
			istringstream( (*row)[sdtblinfo["VarNameID"]]) >> VarNameID;
			istringstream( (*row)[sdtblinfo["Name"]]) >> Name;
			istringstream( (*row)[sdtblinfo["DNP3Index"]]) >> Index;


			_dnp3.DNP3DataList.push_back(new dnp3data(DNP3DataID,DNP3ID,VarNameID,Name,Index));
		}
		DNP3List.push_back(_dnp3);
	}

	/*-------------------------ModbusServer--------------------------------------------*/
	//Filling Table Name
	tblname = "ModbusServer";
	//Reading All DlmsData
	tblinfo.clear();
	tbl = DB.GetTableValues(tblname, NULL);
	tblinfo = DB.GetTableFields(tblname);

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

	for(int i=0;i<rows;i++)
	{
		istringstream( tbl[i][tblinfo["ID"]]) >> ID;
		istringstream( tbl[i][tblinfo["ModbusType"]]) >> ModbusType;
		istringstream( tbl[i][tblinfo["Port"]]) >> Port;
		istringstream( tbl[i][tblinfo["IP"]]) >> IP;
		istringstream( tbl[i][tblinfo["StopBit"]]) >> StopBit;
		istringstream( tbl[i][tblinfo["DataSize"]]) >> DataSize;
		istringstream( tbl[i][tblinfo["Parity"]]) >> Parity;
		istringstream( tbl[i][tblinfo["Baudrate"]]) >> Baudrate;
		istringstream( tbl[i][tblinfo["SerialPortName"]]) >> SerialPortName;
		istringstream( tbl[i][tblinfo["Name"]]) >> Name;
		istringstream( tbl[i][tblinfo["SlaveNumber"]]) >> SlaveNumber;
		istringstream( tbl[i][tblinfo["DiagnosticID"]]) >> DiagnosticID;
		istringstream( tbl[i][tblinfo["ProtocolAddress"]]) >> ProtocolAddress;

		ModbusServer _mbserver(ID,ModbusType,Port,IP,StopBit,DataSize,
				 	 	 	   Parity,Baudrate,SerialPortName, Name,SlaveNumber,
							   DiagnosticID,ProtocolAddress);


		/*-------------------------ModbusServerGroup-----------------------------------------------*/


		string tblname = "ModbusServerGroup";
		vector<vector<string> > sdtbl;
		map<string, int> sdtblinfo;
		multimap<string, string> clause;
		pair<string, string> p("ModbusServerID",tbl[i][tblinfo["ModbusServerID"]]);
		clause.insert(p);
		sdtbl = DB.GetTableValues(tblname,NULL,&clause);
		sdtblinfo = DB.GetTableFields(tblname);

		for(vector<vector<string> >::iterator row = sdtbl.begin(); row != sdtbl.end(); row++)
		{


			uint16_t ID;
			uint16_t ModbusServerID;
			std::string Name;
			uint16_t Start;
			uint16_t Len;
			uint16_t Function;
			uint16_t CycleTime;

			istringstream( (*row)[sdtblinfo["ID"]]) >> ID;
			istringstream( (*row)[sdtblinfo["ModbusServerID"]]) >> ModbusServerID;
			istringstream( (*row)[sdtblinfo["Name"]]) >> Name;
			istringstream( (*row)[sdtblinfo["Start"]]) >> Start;
			istringstream( (*row)[sdtblinfo["Len"]]) >> Len;
			istringstream( (*row)[sdtblinfo["Function"]]) >> Function;
			istringstream( (*row)[sdtblinfo["CycleTime"]]) >> CycleTime;

			_mbserver.ModbusGroup.push_back(make_shared<ModbusServerGroup>(ID,ModbusServerID,Name,Start,
					                                                       Len,Function,CycleTime));

			/*-------------------------ModbusServerPoint-----------------------------------------------*/
			string tblname2 = "ModbusServerGroup";
			vector<vector<string> > sdtbl2;
			map<string, int> sdtblinfo2;
			multimap<string, string> clause2;
			pair<string, string> p("ServerGroupID",tbl[i][sdtblinfo["ID"]]);
			clause2.insert(p);
			sdtbl2 = DB.GetTableValues(tblname2,NULL,&clause2);
			sdtblinfo2 = DB.GetTableFields(tblname);
		}

	}
	return true;
}

Rtdb::~Rtdb() {

}

