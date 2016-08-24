
#include "database.h"
#include <iostream>

int SqlHelper::NumberOfInstance = 0;

SqlHelper::SqlHelper(char* filename)
{
	NumberOfInstance++;
	database = NULL;
    if( !open(filename) )
    {
    	throw("Open Database Error.");

    }
}

SqlHelper::~SqlHelper()
{
	if(database != NULL)
	{
		close();
	}
	NumberOfInstance--;
}

bool SqlHelper::open(char* filename)
{
    if(sqlite3_open(filename, &database) == SQLITE_OK)
        return true;

    return false;
}

vector<vector<string> > SqlHelper::query(char* query)
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

bool SqlHelper::command(char* query)
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

bool SqlHelper::getautocommit()
{
	return !(sqlite3_get_autocommit(database) > 0);
}



map<string,int> SqlHelper::getcolnamesmap(char* query)
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

void SqlHelper::close()
{
	int sqliteApi = sqlite3_close(database);

	if(	sqliteApi != SQLITE_OK)
	{
		cout << "Error In Closing Database File: " << sqlite3_errmsg(database);;
	}
}

int SqlHelper::getError()
{
	return sqlite3_errcode(database);
}


database::database(string filename)
{
	Helper = NULL;
	try
	{
		Helper = new SqlHelper( (char*)filename.c_str() );
	}
	catch(const char* message)
	{
		cout << message;
		delete Helper;
		Helper = NULL;
		throw("DBAccess Failed To Initialize\n");
	}

}

database::~database()
{
	if(Helper != NULL)
	{
		delete Helper;
	}
}

std::string database::GetSQLOperatorString(SQLOperator op)
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

vector< vector<string> > database::GetTableValues(
			string &tblname,
			vector<string>* reqfields /*= NULL*/,
			multimap<string,string>* clause /*= NULL*/,
			vector<SQLOperator>* conditions /*= NULL*/,
			string* additional /*=NULL*/)
{
	string query;

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
							op = GetSQLOperatorString((SQLOperator)(*conditions)[idx]);
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
	return Helper->query((char*) query.c_str());

}

map<string,int> database::GetTableFields(string &tblname)
{
	string query = "SELECT * FROM ";
	query+= tblname;
	return Helper->getcolnamesmap((char*)query.c_str());

}

map<string,int> database::GetQueryFields(string &query)
{
	return Helper->getcolnamesmap((char*)query.c_str());
}

//Set Data base in Immediate Transaction mode
bool database::SetImmediate()
{
	if(Helper == NULL)
	{
		return false;
	}
	return Helper->command((char*)"BEGIN IMMEDIATE TRANSACTION");
}

//Commit all transactions
bool database::Commit()
{
	if(Helper == NULL)
	{
		return false;
	}

	return Helper->command((char*)"COMMIT TRANSACTION");
}

bool database::IsTransactionActive()
{
	return Helper->getautocommit();
}

bool database::TableExist(string& tablename)
{
	string sqlcmd = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tablename + "';";
	vector<vector<string> > tbl = Helper->query((char*)sqlcmd.c_str());
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
