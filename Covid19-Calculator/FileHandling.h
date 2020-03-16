#pragma once
#include"Common.h"

struct File_t
{
    File_t(std::string _file)
        :
        Fullname(_file)
    {}

    std::string Filename;
    std::string Filepath;
    std::string Fullname;
};    