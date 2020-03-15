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
};