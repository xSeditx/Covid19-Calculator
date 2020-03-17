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

        //read_Line();
    }

    void OpenFile(std::string _file)
    {
        FileStream.open(_file.c_str());
        Filesize = get_filesize(_file.c_str());
        if (!FileStream || !Filesize)
        {
            ERROR_LOG("Error Loading File or File is Empty: \n" << _file, file_NotFound);
        }
    }

    std::string read_Line()
    {
        std::string line;
        getline(FileStream, line);
        return line;
    }

    size_t Filesize{ 0 };
    std::string Filename;
    std::string Filepath;
    std::string Fullname;
    std::string Extension;
    std::ifstream FileStream;

private:
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



class CSV_Parser
{
public:

    File_t CSVFile;
    CSV_Parser(std::string _file)
    {
        CSVFile = File_t(_file);
        std::string line;

        /* Gets the Header Information */
        std::string Head = CSVFile.read_Line();
        Header = parse_Line(Head);

        /* Extracts all the Lines from the File */
        for ( ;getline(CSVFile.FileStream, line); )
        {
            Lines.emplace_back(line);
        }

        /* Parses the Lines and places them into appropriate Column of the Vector */
        for (auto& L : Lines)
        {
            Parsed_data.emplace_back(parse_Line(L));
        }
    }

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


    std::string split_str(std::string &_input, char _delim = ',')
    {
        std::string result;

        size_t Field_Length{ 0 };

        size_t End = _input.find(_delim, 1);
        Field_Length = End;
 
        result = _input.substr(0, Field_Length);
        if (result.find('"') != std::string::npos)
        {
            result = parse_quote(_input);
            size_t Next = _input.find(_delim, 0) + 1;
            _input = _input.substr(Next);

            return result;
        }
        size_t Next = _input.find(_delim, 0) + 1;

        _input = _input.substr(Next);

        if (End == std::string::npos)
        {
            result = _input;
            _input = "EOL";
        }
        return result;
    }


    std::vector<std::string> parse_Line(std::string _line)
    {
        std::vector<std::string> result;
        std::string Field;

        size_t Start = _line.find(',', 0);

        while ( _line != "EOL")
        {// Keep going until we read end of Line
            result.push_back(split_str(_line));
        }
        return result;
    }


    std::vector<std::string> Header;
    std::vector<std::string> Rows;
    std::vector<std::string> Lines;

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

