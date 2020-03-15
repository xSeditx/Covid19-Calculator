#include"Covid19calc.h"


std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Configuration Config("Config.txt");



int main()
{

    Pandemic_Map Map;
    Map.load_OutbreakData("Cases.csv");
    {}
    return 0;
}