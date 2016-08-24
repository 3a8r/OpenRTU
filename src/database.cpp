
#include "database.h"
#include <iostream>

int database::NumberOfInstance = 0;


bool database::Open(const char* filename)
{
    if(sqlite3_open(filename, &db) == SQLITE_OK)
        return true;

    return false;
}

vector<vector<string> > database::Query(char* query)
{
    sqlite3_stmt *statement;
    vector<vector<string> > results;
    if(sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
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

    string error = sqlite3_errmsg(db);
    if(error != "not an error") cout << query << " " << error << endl;

    return results;
}

bool database::Command(char* query)
{
    sqlite3_stmt *statement;
    bool res = false;
    if(sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
    {
        int result = 0;
        result = sqlite3_step(statement);
        if(result == SQLITE_DONE)
        {
        	res = true;
        }
        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(db);
    if(error != "not an error") cout << query << " " << error << endl;

    return res;
}

bool database::getautocommit()
{
	return !(sqlite3_get_autocommit(db) > 0);
}



map<string,int> database::getcolnamesmap(char* query)
{
	sqlite3_stmt *stmt;
	map<string,int> values;

	if( sqlite3_prepare_v2(db, query, -1, &stmt, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(stmt);
		for(int col = 0; col < cols; col++)
		{
			values[string((char*)sqlite3_column_name(stmt, col))] = col;
		}

		sqlite3_finalize(stmt);
	}

	string error = sqlite3_errmsg(db);
	if(error != "not an error") cout << query << " " << error << endl;

	return values;


}

void database::close()
{
	int sqliteApi = sqlite3_close(db);

	if(	sqliteApi != SQLITE_OK)
	{
		cout << "Error In Closing Database File: " << sqlite3_errmsg(db);;
	}
}

int database::getError()
{
	return sqlite3_errcode(db);
}


database::database(std::string filename)
{

	try
	{
		NumberOfInstance++;
		db = NULL;
	    if( !Open(filename.c_str()) )
	    {
	    	throw("Open Database Error.");

	    }
	}
	catch(const char* message)
	{
		if(db != NULL)
		{
			close();
		}
		cout << message;
		throw("DBAccess Failed To Initialize\n");
	}

}

database::~database()
{
	if(db != NULL)
	{
		close();
	}
	NumberOfInstance--;
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
	return Query((char*) query.c_str());

}

map<string,int> database::GetTableFields(string &tblname)
{
	string query = "SELECT * FROM ";
	query+= tblname;
	return getcolnamesmap((char*)query.c_str());

}

map<string,int> database::GetQueryFields(string &query)
{
	return getcolnamesmap((char*)query.c_str());
}

//Set Data base in Immediate Transaction mode
bool database::SetImmediate()
{
	if(db == NULL)
	{
		return false;
	}
	return Command((char*)"BEGIN IMMEDIATE TRANSACTION");
}

//Commit all transactions
bool database::Commit()
{
	if(db == NULL)
	{
		return false;
	}

	return Command((char*)"COMMIT TRANSACTION");
}

bool database::IsTransactionActive()
{
	return getautocommit();
}

bool database::TableExist(string& tablename)
{
	string sqlcmd = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tablename + "';";
	vector<vector<string> > tbl = Query((char*)sqlcmd.c_str());
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
