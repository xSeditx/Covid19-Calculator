#pragma once
#include"Common.h"

/* Enumerated Error Types */
enum Error_t
{
    No_Error,
    file_NotFound,
    Extra_column_found
};
extern std::string ErrorString[];

/* Basic Error handler. 
   NOTE: Likely to remove in the near future  */
struct ErrorHandler
{

    void set_ErrorCode(Error_t _er)
    {
        CurrentError = _er;
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