
#ifndef DATABASE_H
#define DATABASE_H


#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"

using namespace std;

class SqlHelper
{
public:
	SqlHelper(char* filename);
    ~SqlHelper();

    bool open(char* filename);
    vector<vector<string> > query(char* query);
    bool command(char* query);
    bool getautocommit();
    map<string,int> getcolnamesmap(char* query);
    void close();
	int getError();

private:
    sqlite3 *database;
public:
	static int NumberOfInstance;
};


using namespace std;

class database{
public:
	typedef enum{

		Equal,
		NotEqual,
		GreaterThan,
		LessThan,
		GreaterThanOrEqual,
		LessThanOrEqual,
		Between,
		LIKE,
		IN

	} SQLOperator;

private:
	SqlHelper* Helper;
	std::string GetSQLOperatorString(SQLOperator op);

public:

	database(string filename);
	~database();
	static int GetNumberOfAccess()
	{
		return SqlHelper::NumberOfInstance;
	}
	vector< vector<string> > GetTableValues(
			string &tblname,
			vector<string>* reqfields = NULL,
			multimap<string,string>* clause = NULL,
			vector<SQLOperator>* conditions = NULL,
			string* additional = NULL);

	map<string,int> GetTableFields(string &tblname);
	map<string,int> GetQueryFields(string &query);

	bool SetImmediate();
	bool Commit();
	bool IsTransactionActive();
	bool TableExist(string& tablename);

	bool ExecuteCommand(string& command)
	{
		return Helper->command((char*)command.c_str());
	}
	int GetError()
	{
		return Helper->getError();
	}

};


#endif /* SqlHelper_H_ */
