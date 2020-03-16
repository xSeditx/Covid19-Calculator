#pragma once
#include"Common.h"
#include<fstream>
#include<sstream>

/* Might get rid of but for now only way to parse daily case data since
fast-csv parse requires known amount of fields at compile time */
struct Parser
{
    Parser(std::string _file)
    {
        std::ifstream t(_file);
        std::stringstream buffer;
        buffer << t.rdbuf();

        std::string deliminator = ",";
    }

    std::vector<std::string> load_Header(std::string _file)
    {

    }
};

//
//http://freerangestats.info/blog/2016/02/06/world-health-organization
// CDC Javascript API/
//file:///C:/Users/Sedit/Downloads/API_User_Guide.pdf