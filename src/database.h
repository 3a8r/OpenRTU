
#ifndef DATABASE_H
#define DATABASE_H


#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"

using namespace std;

class database{
public:
    bool Open(const char* filename);

    vector<vector<string> > Query(char* query);

    bool Command(char* query);

    bool getautocommit();

    map<string,int> getcolnamesmap(char* query);

    void close();

    int getError();

private:
    sqlite3 *db;
public:
	static int NumberOfInstance;
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
	std::string GetSQLOperatorString(SQLOperator op);

public:

	database(string filename);
	~database();
	static int GetNumberOfAccess()
	{
		return NumberOfInstance;
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
		return Command((char*)command.c_str());
	}
	int GetError()
	{
		return getError();
	}

};


#endif /* SqlHelper_H_ */
