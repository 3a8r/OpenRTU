/*
 * rtdb.h
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */
#include <string>
#include <vector>
#ifndef OPENRTU_SRC_RTDB_H_
#define OPENRTU_SRC_RTDB_H_
#include "variable.h"
#include "dnp3.h"
class Rtdb {

public:
	static Rtdb* Instance();
	bool LoadDatabase();

private:
	static Rtdb* instance;

private:
	std::string ProjectPath;
	std::string ConfigFileName;

	// RTUConfig Fields
	int  ID;
	int  Revision;
	std::string Name;
	std::string Description;


	// Variable List
	vector<variable*> VariableList;


	// DNP3 Variable List
	vector<dnp3> DNP3List;

public:
	Rtdb();
	bool LoadConfigFile();

	void SetProjectPath(std::string val){ProjectPath = val;}
	std::string GetProjectPath(){return ProjectPath;}

	void SetConfigFileName(std::string val){ConfigFileName = val;}
	std::string GetConfigFileName(){return ConfigFileName;}

	virtual ~Rtdb();


};

#endif /* OPENRTU_SRC_RTDB_H_ */
