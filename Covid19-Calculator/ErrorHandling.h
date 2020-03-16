#pragma once
#include"Common.h"

enum Error_t
{
    No_Error,
    file_NotFound,
    Extra_column_found
};
extern std::string ErrorString[];
struct ErrorHandler
{

    void set_ErrorCode(Error_t _er)
    {
        CurrentError = _er;
        //ErrorCode = (int)_er;
    }
    std::string get_Error(Error_t _code)
    {
         return ErrorString[(int)_code];
    }
    bool Check_Errors()
    {
        return !(CurrentError == No_Error);
    }
    static Error_t CurrentError;
};