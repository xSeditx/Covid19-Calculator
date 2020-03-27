#pragma once
#include"Common.h"

#include<fstream>
#include<sstream>

#include <filesystem>
namespace fs = std::filesystem;



struct File_t
{
    File_t() = default;
    File_t(std::string _file)
        :
        Fullname(_file)
    {
        OpenFile(_file);

        fs::path Fp = fs::path(_file);

        Filename = Fp.filename().string();
        Filepath = Fp.parent_path().string();
        Extension = Fp.extension().string();
        FileStream.clear();
    }

    /* Opens the Associated File */
    void OpenFile(std::string _file)
    {
        FileStream.open(_file.c_str());
        Filesize = get_filesize(_file.c_str());
        if (!FileStream || !Filesize)
        {
            ERROR_LOG("Error Loading File or File is Empty: \n" << _file, file_NotFound);
        }
    }
    /* Closes the Associated File */
    void CloseFile()
    {
        FileStream.close();
    }

    /* Reads a Single Line from the File Stream */
    std::istream& read_Line(std::string& _line)
    {
        return getline(FileStream, _line);
    }

    /* Returns the Name of the File */
    std::string get_Name() { return    Filename; }
    /* Returns the Path of the File */
    std::string get_Path() { return    Filepath; }
    /* Returns the Full Name and Path of File */
    std::string get_FullName() { return    Fullname; }
    /* Returns the Type Associated with the File*/
    std::string get_Extension() { return    Extension; }

    /* Returns the Working Stream of the Given File*/
    std::ifstream& get_Stream() { return FileStream; }

    /* Returns the Size of the File in Bytes */
    size_t size() const { return Filesize; }
private:
    size_t Filesize{ 0 };

    std::string Filename;
    std::string Filepath;
    std::string Fullname;
    std::string Extension;
    std::ifstream FileStream;

    size_t get_filesize(const char* filename)
    {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return (size_t)in.tellg();
    }
    size_t get_filesize(std::ifstream& _in)
    {
        return (size_t)_in.tellg();
    }
};    

//std::string Head;// = CSVFile.read_Line();


/* ==================================================== 
    Open and Parse a CSV file seperating the Columns 
    into a Vector as a set of strings. 
    ~* WARNING *~ 
    It's the users responsibility to know the format
    of the string to convert it to being string, int,
    or float.
    ==================================================== */
class CSV_Parser
{
public:
        std::vector<std::string> Lines;

    CSV_Parser(std::string _file)
    {
        CSVFile = File_t(_file);
        std::string Line;
        /* Gets the Header Information */
        CSVFile.read_Line(Line);
        Header = parse_Line(Line);
        Column_count = Header.size();
        /* Extracts all the Lines from the File */
        while (CSVFile.read_Line(Line)) 
        {
            Lines.emplace_back(Line);
        }

        /* Parses the Lines and places them into appropriate Column of the Vector */
        for (auto& L : Lines)
        {
            Parsed_data.emplace_back(parse_Line(L));
        }
    }

    /* Properly parse a row with a Quoted Comma */
    std::string parse_quote(std::string &_input)
    {
        std::string result;

        size_t Field_Length{ 0 };

        size_t First = _input.find('"', 0);
        size_t Second = _input.find('"', First+1);
        size_t Last = _input.find(',', Second + 1);

        Field_Length = Second - First;
        if (Second == std::string::npos)
        {
            Field_Length = Last - First;
        }
        result = _input.substr(First+1, Field_Length-1);

      

        _input = _input.substr(Last);
        return result;
    }

    /* Split the string returning the ripped off portion and the remainder in Input */
    std::string split_str(std::string &_input, char _delim = ',')
    {
        std::string result;

        size_t Field_Length{ 0 };

        size_t End = _input.find(_delim, 1);
        Field_Length = End;

        result = _input.substr(0, Field_Length);
        if (result.find('"') != std::string::npos)
        {// If there is Quotation mark we must seperate it
            result = parse_quote(_input);
            size_t Next = _input.find(_delim, 0) + 1;
            _input = _input.substr(Next);
            return result;
        }
        size_t Next = _input.find(_delim, 0) + 1;

        _input = _input.substr(Next);

        if (End == std::string::npos)
        {// Returns a string that signals the end of the line for Parse
            result = _input;
            _input = "[EOL]";
        }
        return result;
    }

    /* Parses the line returning a Vector of the Values as strings */
    std::vector<std::string> parse_Line(std::string _line)
    {
        std::vector<std::string> result;

        size_t Start = _line.find(',', 0);

        while ( _line != "[EOL]")
        {// Keep going until we read end of Line
            std::string PushLine = split_str(_line);
            if (PushLine == ",")
            {
                PushLine = "";
            }
            result.push_back(PushLine);
        }
        return result;
    }


    File_t CSVFile;
    uint32_t Column_count;

    std::vector<std::string> Header;
    std::vector< std::vector<std::string>> Parsed_data;
};
 






//size_t Field_Length{ 0 };
//
//size_t End = _line.find(',', 1 );
//Field_Length = End;
//Field = _line.substr(0, Field_Length  );
//
//_line = _line.substr(_line.find(',', 0 ) + 1);
//

