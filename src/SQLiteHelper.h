/*
 * Database.h
 *
 *  Created on: Jul 29, 2013
 *  Author: a.baseghi
 */

#ifndef SQLiteHelper_H_
#define SQLiteHelper_H_


#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"

using namespace std;

class SQLiteHelper
{
public:
	SQLiteHelper(char* filename);
    ~SQLiteHelper();

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


#endif /* SQLiteHelper_H_ */
