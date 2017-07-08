#define ELPP_DEFAULT_LOG_FILE ( ".\\log.log" )
#include <iostream>
#include "Importer.h"
using namespace std;

INITIALIZE_EASYLOGGINGPP

int checkOption(char* pszOption);
int main(int argc, char* argv[])
{
	el::Configurations conf("log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	//Option
	std::string strServer = "";
	std::string strDatabase = "";
	std::string strOldDatabase = "";
	std::string strRoadDatabase = "";
	std::string strFile = "";
	int index = 0;
	for(index = 1; index < argc - 1; index += 2)
	{
		if(0 == strlen(argv[index + 1]))
		{
			return -1;
		}
		switch(checkOption(argv[index]))
		{
		case Option::Server:
			strServer = argv[index + 1];
			continue;
		case Option::Database:
			strDatabase = argv[index + 1];
			continue;
		case Option::OldDatabase:
			strOldDatabase = argv[index + 1];
			continue;
		case Option::RoadDatabase:
			strRoadDatabase = argv[index + 1];
			continue;
		case Option::File:
			strFile = argv[index + 1];
			continue;
		default:
			continue;
		}
	}
	LOG(INFO) << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
	LOG(INFO) << "Start...\n";
	Importer* p_ssci = new Importer(strServer, strDatabase, strRoadDatabase, strFile, strOldDatabase);
	if(!p_ssci->go())
	{
		return -1;
	}
	if(NULL != p_ssci)
	{
		delete p_ssci;
		p_ssci = NULL;
	}
	system("pause");
	return 0;
}
int checkOption(char* pszOption)
{
	if(0 == strcmp("-s", pszOption))
	{
		return Option::Server;
	}
	if(0 == strcmp("-d", pszOption))
	{
		return Option::Database;
	}
	if(0 == strcmp("-o", pszOption))
	{
		return Option::OldDatabase;
	}
	if(0 == strcmp("-r", pszOption))
	{
		return Option::RoadDatabase;
	}
	if(0 == strcmp("-f", pszOption))
	{
		return Option::File;
	}
	return Option::Other;
}