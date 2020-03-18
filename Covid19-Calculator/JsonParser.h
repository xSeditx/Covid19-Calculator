#pragma once
#include"Common.h"
#include"FileHandling.h"
/* Very likely for this to be removed in the very near future. Attempting a simple JSON parser in hopes to load up a large body of Scientific data I have */



struct Object_base
{
    Object_base(std::string _type) : Type(_type) {}
    std::string Type = "Unknown";
};

template<typename _Ty>
struct Object
    :  Object_base
{
    Object(_Ty _data)
        :
        Object_base(typeid(_data).name()),
        Data(_data)
    {}

    using data_type = _Ty;

    _Ty Data;
};

struct Json
{
    std::vector<Object_base> Objects;
    std::string FullText;
    File_t File;

    Json(std::string _file)
        :
        File(_file),
        RecursionCount(0)
    {
        parse_File();
    }

///        EndPosition = File.size();
/// char *Buffer;
/// size_t ReadPos{ 0 };
/// size_t EndPosition{ 0 };
/// char get_Char()
/// {
///     return Buffer[ReadPos++];
/// }
/// char peek_Char(size_t _pos)
/// {
///     return Buffer[_pos];
/// }
/// const char *get_Line()
/// {
///     size_t Length{ 0 };
///     while ((peek_Char(Length) != '\n')) { ++Length; }
///     const char* result = new char[Length];
///     memcpy(&result, &Buffer[0], Length);
///     Buffer = &Buffer[Length];
///     return result;
/// }
/// const char* SubString(const char *_input, size_t _offset)
/// {
///     size_t Size = EndPosition - _offset;
///     const char* result = new char[Size];
///     memcpy(&result, &Buffer[_offset], Size);
///     Buffer = &Buffer[_offset];
///     return result;
/// }
/// const char* SubString(const char *_input, size_t _start, size_t _end)
/// {
///     size_t Size{ EndPosition };
///
///     size_t Start{ 0 };
///     size_t End{ 0 };
///
///     if
///         (_end > _start)
///     {
///         Start = _end;
///         End = _start;
///     }
///     else
///     { 
///         Start = _start;
///         End = _end;
///     }
///     Size = Start - End;
///
///     const char* result = new char[Size];
///     memcpy(&result, &Buffer[Start], Size);
///     return result;
/// }


    void parse_File()
    {
        std::string Line;

        while (File.read_Line(Line) && RecursionCount == 0)
        {
            FullText += Line;
            Lines.push_back(Line);
        }
        std::string Buffer;
        std::vector<std::string> Objects;
        Objects.push_back(" ");
        std::string O;
        for (auto& L : Lines)
        {
            size_t FirstIn = L.find('{');
            size_t FirstOut = L.find('}');

            if (FirstIn == std::string::npos && FirstOut == std::string::npos)
            {// No { nor } in this line. Add it to the Buffer and Get another Line
                Objects[RecursionCount] += L;
                continue;
            }
            else
            {// There is a Bracket, Find out which is First
                if (FirstOut < FirstIn)
                {// Ideal because our Object Is Closed
                    Objects.push_back(Buffer);
                    Buffer = "";
                    --RecursionCount;
                }
                else
                {
                    Objects.push_back(" ");
                    ++RecursionCount;
                }


                //if (FirstOut == std::string::npos && RecursionCount == 0)
                //{// There are no } in the Line
                //    Buffer += L.substr(0, FirstOut);
                //    continue;
                //}
                //
                //if (FirstIn != std::string::npos)
                //{// There are no { in the Line
                //    ++RecursionCount;
                //    continue;
                //}
            }   //

            if (RecursionCount == -1) break;
        }
        //get_Object(FullText);
        for(auto& A: Objects)
        {
            Print("OBJECT: " << A);
        }
    }
    std::string get_Object(std::string _input)
    {
         std::string result;
         std::vector<std::string>  Buffer;

        size_t FirstIn = _input.find('{');
        size_t FirstOut = _input.find('}');
        _input = _input.substr(FirstIn + 1);
        if (FirstIn == std::string::npos || FirstOut == std::string::npos)
        {
            __debugbreak();
        }
        if (FirstIn < FirstOut)
        {
            ++RecursionCount;
            std::string H = get_Object(_input) + " ";
            Buffer.push_back(H);
        }
        else
        {
            --RecursionCount;
            if (Buffer.size())
            {
               
                for (auto& S : Buffer)
                {
                    result += S;
                }
                return result;
            }
            else
            {
                return result;
            }
        }
    }

    std::string split_str(std::string &_input, char _delim = '{')
    {
    }

    std::vector<std::string> parse_Line(std::string _line)
    {
        std::vector<std::string> result;
        std::string Field;

        size_t Start = _line.find(',', 0);

        while (_line != "EOL")
        {// Keep going until we read end of Line
            result.push_back(split_str(_line));
        }
        return result;
    }



    std::vector<std::string> Lines;
    std::unordered_map<std::string, Object_base*> Data;

    int RecursionCount;
};












//std::vector<std::string> Test;
//Test.push_back(FullText);
//std::string  Pos = FullText.substr(0, FullText.size()); Test.push_back(Pos);
//Buffer = new char[File.size()];
//memcpy(&Buffer, (void*)&FullText, FullText.size());


///std::string result;
///
///size_t Field_Length{ 0 };
///
///size_t End = _input.find(_delim, 1);
///Field_Length = End;
///
///result = _input.substr(0, Field_Length);
///if (result.find('"') != std::string::npos)
///{
///    result = parse_quote(_input);
///    size_t Next = _input.find(_delim, 0) + 1;
///    _input = _input.substr(Next);
///
///    return result;
///}
///size_t Next = _input.find(_delim, 0) + 1;
///
///_input = _input.substr(Next);
///
///if (End == std::string::npos)
///{
///    result = _input;
///    _input = "EOL";
///}
///return result;


// size_t Start = _input.find('{');
// size_t End{0};
// RecursionCount = 0;
// while(RecursionCount == 0)
// { 
//         Obj = new Object(get_Object(_input));
//     }
//     else
//     {
//         --RecursionCount;
//     }
//
//    
//   // _input = _input.substr(Start);
// }
