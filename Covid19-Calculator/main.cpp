#include"Covid19calc.h"
#include"DailyCaseParser.h"

std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Configuration Config("Config.txt");
#include <iostream> 
#include <string> 

int main()
{
    system("git pull origin master");
    Parser Daily("Cases.csv");
    Pandemic_Map Map;
    Map.load_OutbreakData("Cases.csv");
    Map.display_Data();
    {}

    std::string str;
    std::cout << "Please enter your Location: \n";
    std::getline(std::cin, str);
    std::cout << "Outbreak Information for " << str << "\n";
       
    if (Map.is_Local_Outbreaks(str))
    {
        for (auto& P : Map.search_Place(str))
        {
            Print(P);
        }
    }
    else
    {
        Print("No Outbreaks found. Either check spelling of entered location or area is currently clear ");
    }
    return 0;
}

