#include"Covid19calc.h"





int main()
{
   
    //TODO: Parser is escaping on "," Chars despite being explicitly told not to. 
    Pandemic_Map Map;
    Map.load_OutbreakData("Cases.csv");
    {}
    return 0;
}