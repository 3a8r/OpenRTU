/*
 * SQLiteDBAccess.h
 *
 *  Created on: Aug 13, 2013
 *      Author: root
 */

#ifndef SQLITEDBACCESS_H_
#define SQLITEDBACCESS_H_

#include <string>
#include <vector>
#include <map>
#include "SQLiteHelper.h"

using namespace std;

class SQLiteDBAccess{
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
	SQLiteHelper* DBHelper;
	string GetSQLOperatorString(SQLOperator op);

public:

	SQLiteDBAccess(string filename);
	~SQLiteDBAccess();
	static int GetNumberOfAccess()
	{
		return SQLiteHelper::NumberOfInstance;
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
		return DBHelper->command((char*)command.c_str());
	}
	int GetError()
	{
		return DBHelper->getError();
	}

};

#endif /* SQLITEDBACCESS_H_ */
