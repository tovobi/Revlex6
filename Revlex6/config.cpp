#include "stdafx.h"
#include "config.h"

using namespace std;




template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}



static std::map<std::string, std::vector<std::string>> cfgData;

Config::Config()
{
}


Config::~Config()
{
}


bool Config::cfgLoad()
{
	string line;
	//std::map<std::string, vector<std::string>> cfgData;
	ifstream cfgFile("config.cfg");
	if (cfgFile.is_open())
	{
		while (getline(cfgFile, line))
		{
			std::vector<std::string> keyAndValue = split(line, '=');
			cout << keyAndValue[0] << " : " << keyAndValue[1] << endl;
			std::vector<std::string> valueArray = split(keyAndValue[1], ',');
			cout << "size: " << valueArray.size() << endl;
            cfgData.insert({{keyAndValue[0]},{valueArray}});
		}
		cfgFile.close();
        for(auto it = cfgData.cbegin(); it != cfgData.cend(); ++it)
        {
            std::cout << it->first << " " << "\n";
            for (auto x : it->second)
            {
                cout << "\t" << x << endl;
            }

        }
		return true;
	}
	else
	{
		cout << "Unable to read file";
		return false;
	}
}
int Config::getWindowPosX()
{
    if (cfgData.size() > 0)
    {
        std::vector<std::string> x = cfgData["windowPosX"];
        return std::stoi(x[0]);
    }
    else
    {
        return 0;
    }
}
int Config::getWindowPosY()
{
    if (cfgData.size() > 0)
    {
        std::vector<std::string> x = cfgData["windowPosY"];
        return std::stoi(x[0]);
    }
    else
    {
        return 0;
    }
}
std::vector<int> Config::getHostileFactionHorde()
{
    if (cfgData.size() > 0)
    {
        std::vector<int> factionToInt;
        std::vector<std::string> x = cfgData["hostileFactionHorde"];
        for (std::string str : x)
        {
            factionToInt.push_back(stoi(str));
        }
        return factionToInt;
    }
    else
    {
        return {0};
    }
}
std::vector<int> Config::getHostileFactionAlliance()
{
    if (cfgData.size() > 0)
    {
        std::vector<int> factionToInt;
        std::vector<std::string> x = cfgData["hostileFactionAlliance"];
        for (std::string str : x)
        {
            factionToInt.push_back(stoi(str));
        }
        return factionToInt;
    }
    else
    {
        return {0};
    }
}


bool Config::cfgSave(int winPosX, int winPosY, std::vector<int> facHorde, std::vector<int> facAlliance)
{
    // packt die Hostile-Vektoren in strings
    stringstream streamFacHorde;
    stringstream streamFacAlliance;
    for (int x : facHorde)
    {
        streamFacHorde << x << ",";
    }
    for (int x : facAlliance)
    {
        streamFacAlliance << x << ",";
    }

    // packt alle Daten in die config.cfg
	ofstream cfgFile("config.cfg");
	if (cfgFile.is_open())
	{
	    cfgFile << "windowPosX=" << winPosX << endl;
	    cfgFile << "windowPosY=" << winPosY << endl;
	    cfgFile << "hostileFactionHorde=" << streamFacHorde.str() << endl;
		cfgFile << "hostileFactionAlliance=" << streamFacAlliance.str() << endl;
		cfgFile.close();
		return true;
	}
	else
	{
		cout << "Unable to save file";
		return false;
	}
}
