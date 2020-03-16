#pragma once
#include"Common.h"
#include<fstream>
#include<sstream>

#include <fstream>
//std::ifstream infile("thefile.txt");


#include <sstream>
#include <string>


/* Might get rid of but for now only way to parse daily case data since
fast-csv parse requires known amount of fields at compile time */
struct Parser
{
    std::ifstream File;

//std::ifstream::pos_type
    size_t get_filesize(const char* filename)
    {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return (size_t)in.tellg();
    }
    size_t get_filesize(std::ifstream _in)
    {
        return (size_t)_in.tellg();
    }

    char* Buffer;
    size_t FileSize{ 0 };
    size_t CurrentReadPos{ 0 };
    Parser(std::string _file)
    {

        FILE* fl = fopen(_file.c_str(), "r");
        FileSize = get_filesize(_file.c_str());
        Print(FileSize);
        if(!FileSize)
        { 
            ERROR_LOG("Error Loading File or File is Empty: \n" << _file, file_NotFound);
        }
        else
        {// Allocate a Read Buffer the Size of FileSize;
            Buffer = (char*)malloc(FileSize);
        }

        if (fl == 0) {
            ERROR_LOG("Error Loading File: \n" << _file, file_NotFound);
        }



            std::ifstream File(_file);
        try{
        }
        catch (...)
        {
            ERROR_LOG("Error Loading File: \n" << _file, file_NotFound);
        }

        std::string line;
        std::getline(File, line);

        std::string deliminator = ",";
    }

    std::vector<std::string> load_Header(std::string _file)
    {

    }
    std::vector<std::string> Column_names;
};

//
//http://freerangestats.info/blog/2016/02/06/world-health-organization
// CDC Javascript API/
//file:///C:/Users/Sedit/Downloads/API_User_Guide.pdf



/* 


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
public:
    std::string const& operator[](std::size_t index) const
    {
        return m_data[index];
    }
    std::size_t size() const
    {
        return m_data.size();
    }
    void readNextRow(std::istream& str)
    {
        std::string line;
        std::getline(str, line);

        std::string cell;
        std::stringstream lineStream(line);

        m_data.clear();
        while (std::getline(lineStream, cell, ','))
        {
            m_data.push_back(cell);
        }

        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        { // If there was a trailing comma then add an empty element.
            m_data.push_back("");
        }
    }
private:
    std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}
//int main()
//{
//    std::ifstream       file("plop.csv");
//
//    CSVRow              row;
//    while (file >> row)
//    {
//        std::cout << "4th Element(" << row[3] << ")\n";
//    }
//}
//

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str, line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while (std::getline(lineStream, cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}





*/