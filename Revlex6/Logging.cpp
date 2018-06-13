#include "stdafx.h"
#include "Logging.h"


Logging::Logging()
{
}


Logging::~Logging()
{
	file.open("C:/downloads/rv6log.txt", std::ofstream::out | std::ofstream::trunc);
	if (!file.is_open())
	{
		std::cout << "file not found" << endl;
	}
	ss << "startLog";
}
void Logging::open()
{
	file.open("C:/downloads/rv6log.txt");
	if (!file.is_open())
	{
		std::cout << "file not found" << endl;
	}
}
void Logging::close()
{
	file.close();
	std::cout << "file closed" << endl;
}
//template<typename T>
//void Logging::operator+(T data)
//{
//	ss << data;
//}
//void Logging::operator+(const std::string str, const std::string str2)
//{
//	ss << str << str2;
//}
//void Logging::operator+(const int i)
//{
//	ss << std::to_string(i);
//}
//void Logging::operator+(const double d)
//{
//	ss << std::to_string(d);
//}
//void Logging::operator+(const float f)
//{
//	ss << std::to_string(f);
//}
//
//void Logging::clear()
//{
//	ss.clear();
//}
//std::string Logging::str()
//{
//	return ss.str();
//}
void Logging::cout()
{
	if (!file.is_open())
	{
		open();
	}
	file << ss.str();
	ss.str("");
}
