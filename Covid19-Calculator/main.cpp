#include"Covid19calc.h"
 
std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Configuration Config("Config.txt");


#include <iostream> 
#include <string> 
#include <string>
#include <iostream>
#include <filesystem>
 
namespace fs = std::filesystem;

_static Error_t ErrorHandler::CurrentError;
std::string ErrorString[] =
{
    "No Errors",
    "File Not Found",
    "To many Colmns in CSV file"
};


#include"FileHandling.h"


int main()
{
    std::vector<std::string> Filenames;
    std::string path = "COVID-19/archived_data/archived_daily_case_updates/";
    for (const auto & entry : fs::directory_iterator(path))
    {
        auto P = fs::path(entry.path()).extension();
        if (entry.path().extension() == ".csv")
        {
            Filenames.push_back(entry.path().string());
        }
        
    }

    CSV_Parser Fi = CSV_Parser("COVID-19/csse_covid_19_data/csse_covid_19_time_series/time_series_19-covid-Confirmed.csv");
   // Parser Parse("COVID-19/csse_covid_19_data/csse_covid_19_time_series/time_series_19-covid-Confirmed.csv");


    Pandemic_Map Total_Pandemic;
    Total_Pandemic.Retrieve_All_Filenames();
    Total_Pandemic.load_All_Archived();
    Total_Pandemic.load_All_DailyReports();


    Epidemic_Map Map;

    std::vector<Epidemic_Map> AllMap;
    for (auto& F : Filenames)
    {
        AllMap.push_back(Epidemic_Map());
        AllMap.back().load_Archived_OutbreakData(F);
    }
    Map.load_Archived_OutbreakData(Filenames[0]);        //who_covid_19_situation_reports/who_covid_19_sit_rep_time_series/who_covid_19_sit_rep_time_series.csv"); //Cases.csv"); //who_covid_19_sit_rep_time_series.csv
    Map.display_Data();
    {}
    for (auto &A : AllMap)
    {
        A.display_Data();
    }
    bool Running{ true };
    while (Running)
    {
        std::string str;
        std::cout << "Please enter your Location: \n";
        std::getline(std::cin, str);
        if (str == "end") Running = false;
        std::cout << "Outbreak Information for " << str << "\n";

        if (AllMap.back().is_Local_Outbreaks(str))
        {
            for (auto& P : AllMap.back().search_Place(str))
            {
                Print(P);
            }
        }
        else
        {
            Print("No Outbreaks found. Either check spelling of entered location or area is currently clear ");
        }
    }
    return 0;
}





















/* =============================================================================================================
                                  TRASH
   =============================================================================================================
   // /COVID-19/who_covid_19_situation_reports/who_covid_19_sit_rep_time_series/who_covid_19_sit_rep_time_series.csv
//REPO
//https://github.com/CSSEGISandData/COVID-19.git

   //#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif

//#include<iostream>
////using namespace std;
//
//std::string get_current_dir() {
//    char buff[FILENAME_MAX]; //create string buffer to hold path
//    GetCurrentDir(buff, FILENAME_MAX);
//    std::string current_working_dir(buff);
//    return current_working_dir;
//}


//"F:\Program Files\Git\Git-cmd.exe clone https://github.com/CSSEGISandData/COVID-19.git"


// std::string dir = get_current_dir();
// system("cd ..");
// dir = get_current_dir();
// system("git.exe clone o-https://github.com/CSSEGISandData/COVID-19.git");// F: / Program Files / Git / git - cmd.exe clone https ://github.com/CSSEGISandData/COVID-19.git"); // ..git pull origin master");
// Parser Daily("Cases.csv");Cases.csv");//"COVID-19/archived_data/archived_daily_case_updates/02-13-2020_2115.csv"


*/