#pragma once

#include"Common.h"
#include"csv.h"

#define Print(x)  std::cout << x << "\n"



/* City State where Outbreak is taking place */
struct Location_t
{
    Location_t() = default;
    Location_t
    (
        std::string _province,
        std::string _region,
        float       _latitude,
        float       _longitude
    )
        :
        Province(_province),
        Region(_region),
        Latitude(_latitude),
        Longitude(_longitude)
    {}

    std::string Province;
    std::string Region;
    float Latitude;
    float Longitude;
};
std::ostream& operator <<(std::ostream& _str, Location_t _place);

/* Year / Day / Month : Time Data */
struct Date_t
{
    Date_t() = default;
    Date_t
    (
        uint32_t _year,
        uint32_t _month,
        uint32_t _day,
        uint32_t _hours,
        uint32_t _minute,
        uint32_t _seconds
    )
        :
        Year(_year),
        Month(_month),
        Day(_day),
        Time({ _hours, _minute, _seconds })
    {}

    uint32_t Year  { 2020 };
    uint32_t Month ;
    uint32_t Day;

    /* Hours / Minutes / Seconds / data */
    struct Time_t
    {
        Time_t() = default;
        Time_t
        (
            uint32_t _hours,
            uint32_t _minute,
            uint32_t _seconds
        )
            :
            Hours(_hours),
            Minute(_minute),
            Seconds(_seconds)
        {}

        uint32_t Hours  { 0 };
        uint32_t Minute { 0 };
        uint32_t Seconds{ 0 };
    }Time;

    static Date_t parse_DateTimeString(std::string _date)
    {
        Date_t result;

        size_t readPos{ 0 };
        std::string delimiter = "-";

        size_t Count = _date.find(delimiter);

        try
        {
            std::string yearStr = _date.substr(0, Count);
            result.Year = std::stoi(yearStr);
        }
        catch (...)
        {
            result.Year = 0;
        }



        try {
            std::string monthStr = _date.substr(5, 2);
            result.Month = std::stoi(monthStr);
        }
        catch (...)
        {
            result.Month = 0;
        }



        try 
        {
            std::string dayStr = _date.substr(8, 2);
            result.Day = std::stoi(dayStr);
        }
        catch (...)
        {
            result.Day = 0;
        }


        size_t TimeStart = _date.find('T') + 1;

        try
        {
            std::string hourStr = _date.substr(TimeStart, 2);
            result.Time.Hours = std::stoi(hourStr);
        }
        catch (...)
        {

            result.Time.Hours = 0;
        }


        try
        {
            std::string minStr = _date.substr(TimeStart + 3, 2);
            result.Time.Minute = std::stoi(minStr);
        }
        catch(...)
        {
            result.Time.Minute = 0;
        }

        std::string secStr = _date.substr(TimeStart+6, 2);
        result.Time.Seconds = std::stoi(secStr);

        return result;
    }
};                    

std::ostream& operator <<(std::ostream& _str, Date_t _time);


struct Configuration
{
    Configuration(std::string _file)
    {
        io::CSVReader<4, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in(_file);
        in.read_header
        (
            io::ignore_extra_column,/// io::ignore_no_column , ///
            "Province/State",
            "Country/Region",
            "Latitude",
            "Longitude"
        );

        std::string DateTimeIN;
        std::string Long, Lat;

        while (
            in.read_row(
                User_Location.Province,
                User_Location.Region,
                Long,
                Lat))
        {
           // User_Location.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);
            User_Location.Longitude = std::stof(Long);
            User_Location.Latitude = std::stof(Lat);
        }
    }
    Location_t User_Location;
};
extern Configuration Config;


/* Location / Confirmed Cases / Deaths / Recovered :  Date data*/
struct Outbreak_info
{
    Outbreak_info() = default;
    Outbreak_info
    (
        Location_t _place,
        uint32_t   _confirmed,
        uint32_t   _deaths,
        uint32_t   _recovered,
        Date_t     _date_Time
    )
        :
        Place(_place),
        Confirmed(_confirmed),
        Deaths(_deaths),
        Recovered(_recovered),
        Date_Time(_date_Time)
    {}

    Location_t Place;
    uint32_t   Confirmed;
    uint32_t   Deaths;
    uint32_t   Recovered;
    Date_t     Date_Time;
};
std::ostream& operator <<(std::ostream& _str, Outbreak_info _time);


class Pandemic_Map
{
public:
    std::vector<Outbreak_info> Outbreak_List;
    std::unordered_map<std::string, std::vector<Outbreak_info>> Outbreak_Map;
    size_t Total_Deaths{ 0 };
    size_t Total_Infected{ 0 };
    size_t Total_Recovered{ 0 };
    size_t Undetermined;

    float Mortality_Rate{ 0.0f };

    void load_OutbreakData(std::string _filename);


    void display_Data();

    bool is_Local_Outbreaks(std::string _location);

    std::vector<Outbreak_info> search_Place(std::string _location);


    private:

        void removeSpaces(std::string& _str)
        {
            int count = 0;
            int numSpaces = 0;
            for (int i = 0; _str[i]; i++)
            {
                if (_str[i] != ' ')
                {
                    _str[count++] = _str[i];
                }
            }
            _str[count] = '\0';
        }
};



/* ======================================================================================================================
/*                                                 NOTES: 
/* ======================================================================================================================
/* 
-- WINDOWS API for getting User Location
/* https://docs.microsoft.com/en-us/uwp/api/windows.devices.geolocation.basicgeoposition
/* 
-- 2019 Novel Coronavirus COVID - 19 (2019 - nCoV) Data Repository by Johns Hopkins CSSE
/* https://github.com/CSSEGISandData/COVID-19
/* 
/* 
/* ======================================================================================================================*/