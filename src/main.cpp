#include <stdio.h>
#include <iostream>
#include "Rtdb.h"

using namespace std;

int main()
{
	Rtdb::Instance()->SetConfigFileName("config.sqlite");
	Rtdb::Instance()->SetProjectPath("/home/saber/Git/RTU/OpenRTU/config/");
	Rtdb::Instance()->LoadConfigFile();

	return 0;
}
