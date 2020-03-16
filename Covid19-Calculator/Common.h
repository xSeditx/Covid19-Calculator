#pragma once

#define NOMINMAX
#include<Windows.h>
#include<mutex>
#include<string>
#include<iostream>
#include<unordered_map>

#include"ErrorHandling.h"

#define DISABLED_WARNINGS 4244 4267
#pragma warning(disable: DISABLED_WARNINGS)

#define CON_DarkBlue 1
#define CON_DarkGreen 2
#define CON_Darkteal 3
#define CON_DarkRed 4
#define CON_DarkPurple 5
#define CON_DarkYellow 6
#define CON_Grey 7
#define CON_DarkGrey 8
#define CON_Blue 9
#define CON_Green 10
#define CON_Teal 11
#define CON_Red 12
#define CON_Purple 13
#define CON_Yellow 14
#define CON_White 15

extern HANDLE hConsole;
extern std::mutex DEBUGMutex;

#define SetColor(x) SetConsoleTextAttribute(hConsole, x)

#define Print(x)  std::cout <<  x << "\n"

#ifdef _DEBUG
#    define ColorPrint(col, x) { SetColor(col);\
                                 Print(x);\
                                 SetColor(7);}
#else
#    define DEBUGPrint(col,x)
#endif 

//  std::thread::id CurrentThread = std::this_thread::get_id();\



#define ERROR_LOG(x, err)   ColorPrint(CON_Red, x); ErrorHandler::CurrentError = err;



#define _static 