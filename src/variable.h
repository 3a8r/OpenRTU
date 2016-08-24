
#ifndef SRC_OBJECTS_VARIABLE_H_
#define SRC_OBJECTS_VARIABLE_H_
#include <string>

using namespace std;

enum Type
{
	FLOAT =0,
	BOOl,
	BYTE
};

struct VARIABLE
{
	float FLOAT;
	bool	BOOL;
	uint8_t BYTE;

};

class variable
{
public:

	uint32_t 	VarNameID;
	string	 	VarName;
	uint32_t	Type;
	string		Description;
	uint32_t	InitialValue;


	VARIABLE	Value;

	variable( uint32_t _VarNameID,string _VarName,uint32_t _Type,string _Description,uint32_t _InitialValue)
	{
		VarNameID = _VarNameID;
		VarName = _VarName;
		Type = _Type;
		Description = _Description;
		InitialValue = _InitialValue;
	}
};
#endif
