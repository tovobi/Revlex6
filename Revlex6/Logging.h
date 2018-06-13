#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class Logging
{
public:
	Logging();
	~Logging();

	stringstream ss;
	ofstream file;
	//template<typename T>
	//void operator+(T data);
	//void operator+(const std::string str);
	//void operator<<(const std::string str, const std::string str2);
	//void operator+=(const std::string str);
	//void operator+(const int i);
	//void operator+(const double d);
	//void operator+(const float f);
	//void clear();
	//std::string str();
	//std::string endl = "\r\n";
	void cout();
	void open();
	void close();



};

