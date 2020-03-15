#include"Covid19calc.h"
#include"DailyCaseParser.h"

std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Configuration Config("Config.txt");

int main()
{
    Parser Daily("Cases.csv");
    Pandemic_Map Map;
    Map.load_OutbreakData("Cases.csv");
    {}
    return 0;
}