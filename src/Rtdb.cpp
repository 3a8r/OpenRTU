/*
 * rtdb.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: saber
 */

#include "Rtdb.h"

#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include "database.h"
using namespace std;

Rtdb* Rtdb::instance = NULL;


Rtdb* Rtdb::Instance(){

	if(instance == NULL){

		instance = new Rtdb();
	}

	return instance;
}

Rtdb::Rtdb() {

}
bool Rtdb::LoadConfigFile(){

	std::string fulldbfilepath = ProjectPath + ConfigFileName;

	printf("\n File:%s",fulldbfilepath.c_str());

	if( ::access(fulldbfilepath.c_str(),F_OK) == -1 )
	{
		printf("\n Can't Find The Specified Config File.\n");
		char i = 0;
		while(::access(fulldbfilepath.c_str(),F_OK) == -1)
		{
			printf("Waiting For Config File ...%c\r",(i==0)?'|':(i==1)?'/':(i==2)?'-':'\\');
			fflush(stdout);
			if(++i == 4) i=0;
			usleep(500000);
		}
		printf("\b\n");
	}

	int fd;
	int filesize;

	if((fd = ::open(fulldbfilepath.c_str(),O_RDONLY)) != -1)
	{
		struct ::stat fst;
		unsigned char buffer[100];
		do{
			::fstat(fd,&fst);
			printf("\File mode = %d file size = %ld ",fst.st_mode,fst.st_size);
			::lseek(fd,0,SEEK_SET);
			::read(fd,buffer,sizeof(buffer));
			int pagesize = (buffer[16]<<8) | (buffer[17]<<16);
			unsigned int numberofpages = buffer[31] + (buffer[30] <<8) + (buffer[29] << 16) + (buffer [28] << 24);
			filesize = pagesize * numberofpages;
			usleep(100000);
		}
		while(filesize != fst.st_size);
		::close(fd);
	}



	database DB(fulldbfilepath);


	// Read RTU Config Table from sqlite file

	std::string tblname = "RTUConfig";
	vector<vector<string> > tbl = DB.GetTableValues(tblname);
	map<string,int> tblinfo = DB.GetTableFields(tblname);
	istringstream( tbl[0][tblinfo["ID"] ] ) >> ID;
	istringstream( tbl[0][tblinfo["Name"] ] ) >> Name;
	istringstream( tbl[0][tblinfo["Description"] ] ) >> Description;
	istringstream( tbl[0][tblinfo["Revision"] ] ) >> Revision;

	return false;
}

Rtdb::~Rtdb() {

}

