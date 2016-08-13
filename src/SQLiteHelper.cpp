/*
 * Database.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: root
 */

#include "SQLiteHelper.h"
#include <iostream>

int SQLiteHelper::NumberOfInstance = 0;

SQLiteHelper::SQLiteHelper(char* filename)
{
	NumberOfInstance++;
	database = NULL;
    if( !open(filename) )
    {
    	throw("Open Database Error.");

    }
}

SQLiteHelper::~SQLiteHelper()
{
	if(database != NULL)
	{
		close();
	}
	NumberOfInstance--;
}

bool SQLiteHelper::open(char* filename)
{
    if(sqlite3_open(filename, &database) == SQLITE_OK)
        return true;

    return false;
}

vector<vector<string> > SQLiteHelper::query(char* query)
{
    sqlite3_stmt *statement;
    vector<vector<string> > results;
    if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
    {
        int cols = sqlite3_column_count(statement);
        int result = 0;
        while(true)
        {
            result = sqlite3_step(statement);

            if(result == SQLITE_ROW)
            {
                vector<string> values;
                for(int col = 0; col < cols; col++)
                {
					char* ValPtr = (char*)sqlite3_column_text(statement, col);					
					values.push_back((ValPtr != NULL) ? ValPtr : "" );
                }
                results.push_back(values);
            }
            else
            {
                break;
            }
        }

        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(database);
    if(error != "not an error") cout << query << " " << error << endl;

    return results;
}

bool SQLiteHelper::command(char* query)
{
    sqlite3_stmt *statement;
    bool res = false;
    if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
    {
        int result = 0;
        result = sqlite3_step(statement);
        if(result == SQLITE_DONE)
        {
        	res = true;
        }
        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(database);
    if(error != "not an error") cout << query << " " << error << endl;

    return res;
}

bool SQLiteHelper::getautocommit()
{
	return !(sqlite3_get_autocommit(database) > 0);
}



map<string,int> SQLiteHelper::getcolnamesmap(char* query)
{
	sqlite3_stmt *stmt;
	map<string,int> values;

	if( sqlite3_prepare_v2(database, query, -1, &stmt, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(stmt);
		for(int col = 0; col < cols; col++)
		{
			values[string((char*)sqlite3_column_name(stmt, col))] = col;
		}

		sqlite3_finalize(stmt);
	}

	string error = sqlite3_errmsg(database);
	if(error != "not an error") cout << query << " " << error << endl;

	return values;


}

void SQLiteHelper::close()
{
	int sqliteApi = sqlite3_close(database);

	if(	sqliteApi != SQLITE_OK)
	{
		cout << "Error In Closing Database File: " << sqlite3_errmsg(database);;
	}
}

int SQLiteHelper::getError()
{
	return sqlite3_errcode(database);
}
