/*
 * SQLiteDBAccess.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: root
 */

#include <iostream>
#include "SQLiteDBAccess.h"

SQLiteDBAccess::SQLiteDBAccess(string filename)
{
	DBHelper = NULL;
	try
	{
		DBHelper = new SQLiteHelper( (char*)filename.c_str() );
		//DBHelper->query("PRAGMA foreign_keys = ON;");
	}
	catch(const char* message)
	{
		cout << message;
		delete DBHelper;
		DBHelper = NULL;
		throw("DBAccess Failed To Initialize\n");
	}

}

SQLiteDBAccess::~SQLiteDBAccess()
{
	if(DBHelper != NULL)
	{
		delete DBHelper;
	}
}

string SQLiteDBAccess::GetSQLOperatorString(SQLOperator op)
{
	switch(op)
	{
	case Equal:
		return "=";
	case NotEqual:
		return "<>";
	case GreaterThan:
		return ">";
	case LessThan:
		return "<";
	case GreaterThanOrEqual:
		return ">=";
	case LessThanOrEqual:
		return "<=";
	case Between:
		return "BETWEEN";
	case LIKE:
		return "LIKE";
	case IN:
		return "IN";
	};

	return "=";

}

vector< vector<string> > SQLiteDBAccess::GetTableValues(
			string &tblname,
			vector<string>* reqfields /*= NULL*/,
			multimap<string,string>* clause /*= NULL*/,
			vector<SQLOperator>* conditions /*= NULL*/,
			string* additional /*=NULL*/)
{
	string query;
	// Ehsan Amid 13-Dec-2013
	if(tblname != "")
	{
		query = "SELECT ";

		if (reqfields != NULL)
		{

			if (reqfields->size() > 0)
			{
				for (vector<string>::iterator walker = reqfields->begin(); walker
					!= reqfields->end(); walker++)
				{
					query += ((*walker) + ",");
				}

				query = query.erase(query.length() - 1);
			}
			else{
				query += "*";

			}
		}
		else
		{
			query += "*";
		}

		query += " FROM " + tblname;

		if(clause != NULL)
		{
			if(clause->size() > 0)
			{
				query+= " WHERE ";
				int idx = 0;
				for (multimap<string, string>::iterator walker = clause->begin(); walker
					!= clause->end(); walker++)
				{
					if(walker != clause->begin())
					{
						query += " OR ";
					}

					string op = "=";
					if(conditions != NULL){
						if(conditions->size() > 0){
							op = GetSQLOperatorString((*conditions)[idx]);
							idx++;
						}

					}
					query += (*walker).first ;
					query += op;
					query += (*walker).second;

				}

			}
		}
		if(additional != NULL)
		{
			query += (" "+(*additional));
		}
	}
	else
	{
		query = (" "+(*additional));
	}
	return DBHelper->query((char*) query.c_str());

}

map<string,int> SQLiteDBAccess::GetTableFields(string &tblname)
{
	string query = "SELECT * FROM ";
	query+= tblname;
	return DBHelper->getcolnamesmap((char*)query.c_str());

}

map<string,int> SQLiteDBAccess::GetQueryFields(string &query)
{
	return DBHelper->getcolnamesmap((char*)query.c_str());
}

//Set Data base in Immediate Transaction mode
bool SQLiteDBAccess::SetImmediate()
{
	if(DBHelper == NULL)
	{
		return false;
	}
	return DBHelper->command((char*)"BEGIN IMMEDIATE TRANSACTION");
}

//Commit all transactions
bool SQLiteDBAccess::Commit()
{
	if(DBHelper == NULL)
	{
		return false;
	}

	return DBHelper->command((char*)"COMMIT TRANSACTION");
}

bool SQLiteDBAccess::IsTransactionActive()
{
	return DBHelper->getautocommit();
}

bool SQLiteDBAccess::TableExist(string& tablename)
{
	string sqlcmd = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tablename + "';";
	vector<vector<string> > tbl = DBHelper->query((char*)sqlcmd.c_str());
	if(tbl.size() == 0)
	{
		return false;
	}
	if(tbl[0].size() == 0)
	{
		return false;
	}
	return (tablename == tbl[0][0]);
}
