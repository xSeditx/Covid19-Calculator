#pragma once
#define _USE_SDL_


#define SCREEN_X 1280
#define SCREEN_Y 720


#define NOMINMAX
#include<Windows.h>
#include<mutex>
#include<stack>
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


#define ERROR_LOG(x, err)   ColorPrint(CON_Red, x); ErrorHandler::CurrentError = err;

#define _static 


#ifdef _USE_OPENGL_
#    include"../Bin/glad/include/glad/glad.h"
#endif


/* =========================== SDL2 Graphics Library  ================================== */
#ifdef _USE_SDL_
#    define SDL_MAIN_HANDLED
     
#    include"SDL2/include/SDL.h"
#    include"SDL2/include/SDL_main.h"
#    include"SDL2/include/SDL_syswm.h"
#    include"SDL2/include/SDL_ttf.h"
//#    include"SDL2/include/nanovg_rt.h"
//#    include"SDL2/include/nanovg.h"
#    include"SDL2/include/common.h"


#    pragma comment(lib, "SDL2/lib/x64/nanogui.lib")
#    pragma comment(lib, "SDL2/lib/x64/SDL2.lib")
#    pragma comment(lib, "SDL2/lib/x64/SDL2main.lib")
#    pragma comment(lib, "SDL2/lib/x64/SDL2_Image.lib")
#    pragma comment(lib, "SDL2/lib/x64/SDL2_TTF.lib")
#endif 
/* ===================================================================================== */







/* ========================== Math and Vector Library ================================== */

#include"glm/glm/glm.hpp"
#include"glm/glm/gtc/type_ptr.hpp"
#include"glm/glm/gtc/matrix_transform.hpp"

// Float Vectors
typedef glm::vec2    Vec2;
typedef glm::vec3    Vec3;
typedef glm::vec4    Vec4;

// Integer Vectors
typedef glm::ivec2  iVec2;
typedef glm::ivec3  iVec3;
typedef glm::ivec4  iVec4;

// Colors
typedef glm::vec3   fRGB;
typedef glm::vec4   fRGBA;

typedef glm::mat3   Mat3;
typedef glm::mat4   Mat4;
/* ===================================================================================== */

using MsgID = unsigned int;
using idTag = size_t;



///std::ostream& operator << (std::ostream &lhv, Vec2 const &rhv);
///std::ostream& operator << (std::ostream &lhv, Vec3 const &rhv);
///std::ostream& operator << (std::ostream &lhv, Vec4 const &rhv);



static std::ostream& operator<<(std::ostream &lhv, Vec2 const &rhv)
{
    lhv << "X: " << rhv.x << " Y: " << rhv.y;
    return lhv;
}
static std::ostream& operator<<(std::ostream &lhv, Vec3 const &rhv)
{
    lhv << "  X: " << rhv.x << " Y: " << rhv.y << " Z: " << rhv.z;
    return lhv;
}
static std::ostream& operator<<(std::ostream &lhv, Vec4 const &rhv)
{
    lhv << " X: " << rhv.x << " Y: " << rhv.y << " Z: " << rhv.z << " W: " << rhv.w;
    return lhv;
}









#define DEFAULT_DRAW_COLOR  155,155,155,155

#define Squared(x)   ((x)*(x))


#define RADIANS(x)            ((x) * 0.01745329251)
#define DEGREES(x)            ((x) * 57.2957795131)





#define for_loop(_itr, _count)         for(uint32_t _itr{0}; _itr < (_count); ++_itr)


#if _DEBUG
/*Basic Macro does as appears and only allows code to appear on Debug Builds */
#    define DEBUG_CODE(_code)  // _code
#else
/* Code Currently turned off */
#    define DEBUG_CODE(_code)   
#endif










/* ========================== BIT Managment Macros  ================================== */

/* Defines for accessing the upper and lower byte of an integer. */
#define  LOW_BYTE(x)    (x & 0x00FF)
#define  HI_BYTE(x)     ((x & 0xFF00) >> 8)

#define  BIT(x)         (1 << x) // GET THE STATE OF A SINGLE BIT AT X POSITION
#define  GET_BIT(value, x) (value & BIT(x))
#define  MAX_INTEGER    0xFFFFFFFF

#define EXTRACT_BIT(value, x)      ((value >> (x)) & ((1 << (x)) - 1)) // Value & Mask
#define TOGGLE_BIT(x)    ^= (1 << x)
#define SET_BIT(x)       |= (1 << x)
#define CLEAR_BIT(x)     &=~(1<<x)

#define  BIT_32         0x80000000         //      DEC 2147483648   BIN:  1000 0000 0000 0000 0000 0000 0000 0000
#define  BIT_31         0x40000000         //      DEC 1073741824	BIN:  0100 0000 0000 0000 0000 0000 0000 0000
#define  BIT_30         0x20000000         //      DEC 536870912	BIN:  0010 0000 0000 0000 0000 0000 0000 0000
#define  BIT_29         0x10000000         //      DEC 268435456	BIN:  0001 0000 0000 0000 0000 0000 0000 0000
#define  BIT_28         0x8000000          //      DEC 134217728	BIN:  0000 1000 0000 0000 0000 0000 0000 0000
#define  BIT_27         0x4000000          //      DEC 67108864	    BIN:  0000 0100 0000 0000 0000 0000 0000 0000
#define  BIT_26         0x2000000          //      DEC 33554432	    BIN:  0000 0010 0000 0000 0000 0000 0000 0000
#define  BIT_25         0x1000000          //      DEC 16777216	    BIN:  0000 0001 0000 0000 0000 0000 0000 0000
#define  BIT_24         0x800000           //      DEC 8388608		BIN:  0000 0000 1000 0000 0000 0000 0000 0000
#define  BIT_23         0x400000           //      DEC 4194304		BIN:  0000 0000 0100 0000 0000 0000 0000 0000
#define  BIT_22         0x200000           //      DEC 2097152		BIN:  0000 0000 0010 0000 0000 0000 0000 0000
#define  BIT_21         0x100000           //      DEC 1048576		BIN:  0000 0000 0001 0000 0000 0000 0000 0000
#define  BIT_20         0x80000            //      DEC 524288		BIN:  0000 0000 0000 1000 0000 0000 0000 0000
#define  BIT_19         0x40000            //      DEC 262144		BIN:  0000 0000 0000 0100 0000 0000 0000 0000
#define  BIT_18         0x20000            //      DEC 131072		BIN:  0000 0000 0000 0010 0000 0000 0000 0000
#define  BIT_17         0x10000	           //      DEC 65536		BIN:  0000 0000 0000 0001 0000 0000 0000 0000
#define  BIT_16         0x8000             //      DEC 32768        BIN:  0000 0000 0000 0000 1000 0000 0000 0000
#define  BIT_15         0x4000             //      DEC 16384        BIN:  0000 0000 0000 0000 0100 0000 0000 0000
#define  BIT_14         0x2000             //      DEC 8192         BIN:  0000 0000 0000 0000 0010 0000 0000 0000
#define  BIT_13         0x1000             //      DEC 4096         BIN:  0000 0000 0000 0000 0001 0000 0000 0000
#define  BIT_12         0x800              //      DEC 2048         BIN:  0000 0000 0000 0000 0000 1000 0000 0000
#define  BIT_11         0x400              //      DEC 1024         BIN:  0000 0000 0000 0000 0000 0100 0000 0000
#define  BIT_10         0x200              //      DEC 512          BIN:  0000 0000 0000 0000 0000 0010 0000 0000
#define  BIT_9          0x100              //      DEC 256          BIN:  0000 0000 0000 0000 0000 0001 0000 0000
#define  BIT_8          0x80               //      DEC 128          BIN:  0000 0000 0000 0000 0000 0000 1000 0000
#define  BIT_7          0x40               //      DEC 64           BIN:  0000 0000 0000 0000 0000 0000 0100 0000
#define  BIT_6          0x20               //      DEC 32           BIN:  0000 0000 0000 0000 0000 0000 0010 0000
#define  BIT_5          0x10               //      DEC 16           BIN:  0000 0000 0000 0000 0000 0000 0001 0000
#define  BIT_4          0x8                //      DEC 8            BIN:  0000 0000 0000 0000 0000 0000 0000 1000
#define  BIT_3          0x4                //      DEC 4            BIN:  0000 0000 0000 0000 0000 0000 0000 0100
#define  BIT_2          0x2                //      DEC 2            BIN:  0000 0000 0000 0000 0000 0000 0000 0010
#define  BIT_1          0x1                //      DEC 1            BIN:  0000 0000 0000 0000 0000 0000 0000 0001


#define  BIT_32_MASK         (~(0x80000000))         // DEC BIN:  0111 1111 1111 1111 1111 1111 1111 1111
#define  BIT_31_MASK         (~(0x40000000))         // DEC BIN:  1011 1111 1111 1111 1111 1111 1111 1111
#define  BIT_30_MASK         (~(0x20000000))         // DEC BIN:  1101 1111 1111 1111 1111 1111 1111 1111
#define  BIT_29_MASK         (~(0x10000000))         // DEC BIN:  1110 1111 1111 1111 1111 1111 1111 1111
#define  BIT_28_MASK         (~(0x8000000 ))         // DEC BIN:  1111 0111 1111 1111 1111 1111 1111 1111
#define  BIT_27_MASK         (~(0x4000000 ))         // DEC BIN:  1111 1011 1111 1111 1111 1111 1111 1111
#define  BIT_26_MASK         (~(0x2000000 ))         // DEC BIN:  1111 1101 1111 1111 1111 1111 1111 1111
#define  BIT_25_MASK         (~(0x1000000 ))         // DEC BIN:  1111 1110 1111 1111 1111 1111 1111 1111
#define  BIT_24_MASK         (~(0x800000  ))         // DEC BIN:  1111 1111 0111 1111 1111 1111 1111 1111
#define  BIT_23_MASK         (~(0x400000  ))         // DEC BIN:  1111 1111 1011 1111 1111 1111 1111 1111
#define  BIT_22_MASK         (~(0x200000  ))         // DEC BIN:  1111 1111 1101 1111 1111 1111 1111 1111
#define  BIT_21_MASK         (~(0x100000  ))         // DEC BIN:  1111 1111 1110 1111 1111 1111 1111 1111
#define  BIT_20_MASK         (~(0x80000   ))         // DEC BIN:  1111 1111 1111 0111 1111 1111 1111 1111
#define  BIT_19_MASK         (~(0x40000   ))         // DEC BIN:  1111 1111 1111 1011 1111 1111 1111 1111
#define  BIT_18_MASK         (~(0x20000   ))         // DEC BIN:  1111 1111 1111 1101 1111 1111 1111 1111
#define  BIT_17_MASK         (~(0x10000	  ))         // DEC BIN:  1111 1111 1111 1110 1111 1111 1111 1111
#define  BIT_16_MASK         (~(0x8000    ))         // DEC BIN:  1111 1111 1111 1111 0111 1111 1111 1111
#define  BIT_15_MASK         (~(0x4000    ))         // DEC BIN:  1111 1111 1111 1111 1011 1111 1111 1111
#define  BIT_14_MASK         (~(0x2000    ))         // DEC BIN:  1111 1111 1111 1111 1101 1111 1111 1111
#define  BIT_13_MASK         (~(0x1000    ))         // DEC BIN:  1111 1111 1111 1111 1110 1111 1111 1111
#define  BIT_12_MASK         (~(0x800     ))         // DEC BIN:  1111 1111 1111 1111 1111 0111 1111 1111
#define  BIT_11_MASK         (~(0x400     ))         // DEC BIN:  1111 1111 1111 1111 1111 1011 1111 1111
#define  BIT_10_MASK         (~(0x200     ))         // DEC BIN:  1111 1111 1111 1111 1111 1101 1111 1111
#define  BIT_9_MASK          (~(0x100     ))         // DEC BIN:  1111 1111 1111 1111 1111 1110 1111 1111
#define  BIT_8_MASK          (~(0x80      ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 0111 1111
#define  BIT_7_MASK          (~(0x40      ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1011 1111
#define  BIT_6_MASK          (~(0x20      ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1101 1111
#define  BIT_5_MASK          (~(0x10      ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1110 1111
#define  BIT_4_MASK          (~(0x8       ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1111 0111
#define  BIT_3_MASK          (~(0x4       ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1111 1011
#define  BIT_2_MASK          (~(0x2       ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1111 1101
#define  BIT_1_MASK          (~(0x1       ))         // DEC BIN:  1111 1111 1111 1111 1111 1111 1111 1110




/* ===================================================================================== */
