#pragma once
#include <fstream>

class LogFile
{
public:
	LogFile();
	~LogFile();


	static std::ofstream logFile;

};

