#include"Covid19calc.h"


std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);




int main()
{
   
    //TODO: Parser is escaping on "," Chars despite being explicitly told not to. 
    Pandemic_Map Map;
    Map.load_OutbreakData("Cases.csv");
    {}
    return 0;
}