#pragma once

#include"Common.h"
#include<string>
#include<iostream>
#include<unordered_map>
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
        std::string yearStr = _date.substr(0, Count);  
        result.Year = std::stoi(yearStr);

        std::string monthStr = _date.substr(5, 2);  
        result.Month = std::stoi(monthStr);

        std::string dayStr = _date.substr(8, 2);  
        result.Day = std::stoi(dayStr);

        size_t TimeStart = _date.find('T') + 1;
        std::string hourStr = _date.substr(TimeStart, 2);
        result.Time.Hours = std::stoi(hourStr);

        std::string minStr = _date.substr(TimeStart+3, 2);
        result.Time.Minute = std::stoi(minStr);

        std::string secStr = _date.substr(TimeStart+6, 2);
        result.Time.Seconds = std::stoi(secStr);

        return result;
    }
};

struct Configuration
{
    Location_t User_Location;
};

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


class Pandemic_Map
{
public:
    std::vector<Outbreak_info> Outbreak_List;
    
    size_t Total_Deaths{ 0 };
    size_t Total_Infected{ 0 };
    size_t Total_Recovered{ 0 };
    float Mortality_Rate{ 0.0f };


    void load_OutbreakData(std::string _filename)
    {
        io::CSVReader<8, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in("Cases.csv");
        in.read_header
        (
           io::ignore_extra_column,/// io::ignore_no_column , ///
            "Province/State",
            "Country/Region",
            "Last Update",
            "Confirmed",
            "Deaths",
            "Recovered",
            "Latitude",
            "Longitude"
        );

        Outbreak_info Row_data;

        std::string DateTimeIN;
        //float Long{ 0 }, Lat{ 0 };
        std::string Long, Lat;
         while (
             in.read_row(
             Row_data.Place.Province,
             Row_data.Place.Region,
             DateTimeIN, 
             Row_data.Confirmed,
             Row_data.Deaths,
             Row_data.Recovered,
             Long,
             Lat))
         {
             Row_data.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);
             Row_data.Place.Longitude = std::stof(Long);
             Row_data.Place.Latitude = std::stof(Lat);
             Outbreak_List.push_back(Row_data);
             

             Total_Deaths += Row_data.Deaths;
             Total_Infected += Row_data.Confirmed;
             Total_Recovered += Row_data.Recovered;

         }

         ColorPrint(CON_Yellow, "Total Infected:  " << Total_Infected);
         ColorPrint(CON_Green, "Total Recovered: " << Total_Recovered);
         ColorPrint(CON_Red, "Total Deaths:    " << Total_Deaths);

         Mortality_Rate = (static_cast<float>(Total_Deaths)/ static_cast<float>(Total_Infected) );
         ColorPrint(CON_Red, "Mortality:       " << (Mortality_Rate * 100.f) << "%");

         /* Figure up the amount of people that Recover for every one death*/
         Print("\n");
         float recov_death = (float)((Total_Recovered) / (Total_Deaths));
         Print("Death/Recovery ratio: 1/" << recov_death);
         Print("\n");

         /* Figure up the amount of cases still to be seen */
         Undetermined = Total_Infected - (Total_Deaths + Total_Recovered);
         ColorPrint(CON_Yellow, "Undetermined: " << Undetermined);
         ColorPrint(CON_Red, "~ Estimated Deaths:    " << Undetermined * Mortality_Rate );
         ColorPrint(CON_Green, "~ Estimated Recoverys: " << Undetermined - (Undetermined * Mortality_Rate));
    }

    float Undetermined;


    
};










//  "Province / State",
//  "Country / Region",
//  "Last Update"     ,
//  "Confirmed"	      ,
//  "Deaths"          ,
//  "Recovered"	      ,
//  "Latitude"        ,
//  "Longitude"       ,

//Province / State	Country / Region	Last Update	Confirmed	Deaths	Recovered	Latitude	Longitude




/* ======================================================================================================================
/*                                                 NOTES: 
/* ======================================================================================================================
/* 
/* 
/* 
-- 2019 Novel Coronavirus COVID - 19 (2019 - nCoV) Data Repository by Johns Hopkins CSSE
/* https://github.com/CSSEGISandData/COVID-19
/* 
/* 
/* ======================================================================================================================*/