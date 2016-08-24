#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "Rtdb.h"

using namespace std;

int main()
{
	// Set Config File Name
	Rtdb::Instance()->SetConfigFileName("config.sqlite");

	// Set Config File Address
	Rtdb::Instance()->SetProjectPath("/home/saber/Git/RTU/OpenRTU/config/");

	// Read Log File
	Rtdb::Instance()->LoadConfigFile();

	while(1)
	{
		sleep(1);
	}
}
