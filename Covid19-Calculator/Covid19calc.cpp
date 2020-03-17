#include"Covid19calc.h"

void Epidemic_Map::display_Data()
{

    ColorPrint(CON_Yellow, "Total Infected:  " << Total_Infected);
    ColorPrint(CON_Green, "Total Recovered: " << Total_Recovered);
    ColorPrint(CON_Red, "Total Deaths:    " << Total_Deaths);

    /* Find the Percentage of people who died so far */
    Mortality_Rate = (static_cast<float>(Total_Deaths) / static_cast<float>(Total_Infected));
    ColorPrint(CON_Red, "Mortality:       " << (Mortality_Rate * 100.f) << "%");

    /* Figure up the amount of people that Recover for every one death*/
    Print("\n");
    float recov_death = (float)((Total_Recovered) / (Total_Deaths));
    Print("Death/Recovery ratio: 1/" << recov_death);
    Print("\n");

    /* Figure up the amount of cases still to be seen */
    Undetermined = Total_Infected - (Total_Deaths + Total_Recovered);
    ColorPrint(CON_Yellow, "Undetermined: " << Undetermined);
    ColorPrint(CON_Red, "~ Estimated Deaths:    " << Undetermined * Mortality_Rate);
    ColorPrint(CON_Green, "~ Estimated Recoverys: " << Undetermined - (Undetermined * Mortality_Rate));

    // is_Local_Outbreaks(Config.User_Location.Province);
}
void Epidemic_Map::load_Archived_OutbreakData(std::string _filename)
{
    io::CSVReader<6, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in(_filename);// "Cases.csv");
    in.read_header
    (
        io::ignore_extra_column,/// io::ignore_no_column , ///
        "Province/State",
        "Country/Region",
        "Last Update",
        "Confirmed",
        "Deaths",
        "Recovered"
    );

    Outbreak_info Row_data;
    std::string DateTimeIN;
    std::string Long, Lat;
    while (
        in.read_row(
            Row_data.Place.Province,
            Row_data.Place.Region,
            DateTimeIN,
            Row_data.Confirmed,
            Row_data.Deaths,
            Row_data.Recovered))
    {

        Row_data.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);
        Row_data.Place.Latitude = 0;
        Row_data.Place.Longitude = 0;

        Outbreak_List.push_back(Row_data);

        Total_Deaths += Row_data.Deaths;
        Total_Infected += Row_data.Confirmed;
        Total_Recovered += Row_data.Recovered;

        Outbreak_Map[Row_data.Place.Province].push_back(Row_data);
    }
}
void Epidemic_Map::load_Daily_OutbreakData(std::string _filename)
{
    try {
        io::CSVReader<8, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in(_filename);
        in.read_header
        (
            io::ignore_extra_column,
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
        std::string Long, Lat;

        while (
            in.read_row(
                Row_data.Place.Province,
                Row_data.Place.Region,
                DateTimeIN,
                Row_data.Confirmed,
                Row_data.Deaths,
                Row_data.Recovered,
                Lat,
                Long
            ))
        {

            Row_data.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);

            try
            {
                Row_data.Place.Latitude = std::stof(Lat);
                Row_data.Place.Longitude = std::stof(Long);
            }
            catch (...)
            {// If stof fails due to poorly implemented data
                Row_data.Place.Latitude = 0;
                Row_data.Place.Longitude = 0;
            }

            Outbreak_List.push_back(Row_data);

            Total_Deaths += Row_data.Deaths;
            Total_Infected += Row_data.Confirmed;
            Total_Recovered += Row_data.Recovered;

            Outbreak_Map[Row_data.Place.Province].push_back(Row_data);
        }

    }
    catch (...)
    {// If the Above Fails which it will for the first Month of Data use the following which excludes Long and Lat data
        io::CSVReader<6, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in(_filename);
        in.read_header
        (
            io::ignore_extra_column,
            "Province/State",
            "Country/Region",
            "Last Update",
            "Confirmed",
            "Deaths",
            "Recovered"
        );
        Outbreak_info Row_data;
        std::string DateTimeIN;
        std::string Long, Lat;

        while (
            in.read_row(
                Row_data.Place.Province,
                Row_data.Place.Region,
                DateTimeIN,
                Row_data.Confirmed,
                Row_data.Deaths,
                Row_data.Recovered
            ))
        {

            Row_data.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);
            Row_data.Place.Latitude = 0;
            Row_data.Place.Longitude = 0;

            Outbreak_List.push_back(Row_data);

            Total_Deaths += Row_data.Deaths;
            Total_Infected += Row_data.Confirmed;
            Total_Recovered += Row_data.Recovered;

            Outbreak_Map[Row_data.Place.Province].push_back(Row_data);
        }

    }

}


bool Epidemic_Map::is_Local_Outbreaks(std::string _location)
{

    std::vector<Outbreak_info> result = search_Place(_location);
    Print("Number of Local Outbreaks: " << result.size());

    for (auto& O : result)
    {
        Print("--------------------------------------");
        Print("OUTBREAK INFORMATION: \n" << O);
        Print("Local Cases: " << O.Confirmed);
        Print("      Recovered: " << O.Recovered);
        Print("      Deaths: " << O.Deaths);
        Print("--------------------------------------");
    }

    return false;
}

std::vector<Outbreak_info>
    Epidemic_Map::search_Place(std::string _location)
{
    std::vector<Outbreak_info> result;
    /* Checks the User Configuration file to see if there are Outbreaks in their Area */
    if (Outbreak_Map.find(_location) != Outbreak_Map.end())
    {// If there are outbreaks Report Each one 
        for (auto& O : Outbreak_Map[_location])
        {
            result.push_back(O);
        }
    }
    /* Double checks the Region to see if the User ment that instead */
    for (auto& O : Outbreak_Map)
    {
        for (auto& C : O.second)
        {
            if (C.Place.Region == _location)
            {
                result.push_back(C);
            }
        }
    }

    // Make sure we do not double the data in the following test
    if (result.size())return result;


    // If all of the above Fails we start string manipulation to see if user possibly entered incorrect Capitalization or Spacing
    // std::for_each(_location.begin(), _location.end(), [](char & c) { c = ::toupper(c); });
    std::string UcaseInput = _location;
    std::for_each(UcaseInput.begin(), UcaseInput.end(), [](char & c) { c = ::toupper(c); });
    removeSpaces(UcaseInput);

    /* Double checks the Region to see if the User ment that instead */
    for (auto& O : Outbreak_Map)
    {
        for (auto& C : O.second)
        {

            std::string UcaseProv = C.Place.Province;
            std::for_each(UcaseProv.begin(), UcaseProv.end(), [](char & c) { c = ::toupper(c); });


            std::string UcaseRegion = C.Place.Region;
            std::for_each(UcaseRegion.begin(), UcaseRegion.end(), [](char & c) { c = ::toupper(c); });

            removeSpaces(UcaseRegion);
            removeSpaces(UcaseProv);

            // We have to take a substring the size of the Input key because for whatever reason removeSpaces is returning extra characters
            // I have to make a better way to remove space because the /0 char is being added at the position where the amount of spaces are
            if (UcaseProv.size())
            {// Only check if Province Field Exist
                if (UcaseProv == (UcaseInput.substr(0, UcaseProv.size())))
                {// The Province equals user input
                    result.push_back(C);
                }
            }

            if (UcaseRegion.size())
            {// Only Check if Region Field Exist
                if (UcaseRegion == (UcaseInput.substr(0, UcaseRegion.size())))
                {// The Region Matches user input
                    result.push_back(C);
                }
            }
        }
    }

    // If that STILL does not work we are going to attempt to find the users input as a Substring to the Region or Province
    if (result.size())return result;


    /* Tripple checks the Region and Province to see if the User gave a substring of the area */
    for (auto& O : Outbreak_Map)
    {
        for (auto& C : O.second)
        {

            std::string UcaseProv = C.Place.Province;
            std::for_each(UcaseProv.begin(), UcaseProv.end(), [](char & c) { c = ::toupper(c); });

            std::string UcaseRegion = C.Place.Region;
            std::for_each(UcaseRegion.begin(), UcaseRegion.end(), [](char & c) { c = ::toupper(c); });

            removeSpaces(UcaseRegion);
            removeSpaces(UcaseProv);

            // We have to take a substring the size of the Input key because for whatever reason removeSpaces is returning extra characters
            // I have to make a better way to remove space because the /0 char is being added at the position where the amount of spaces are
            if (UcaseProv.size())
            {// Only check if Province Field Exist
                if (UcaseProv.find(UcaseInput) != std::string::npos)
                {// The Province equals user input
                    result.push_back(C);
                }
            }

            if (UcaseRegion.size())
            {// Only Check if Region Field Exist
                if (UcaseRegion.find(UcaseInput) != std::string::npos)
                {// The Region Matches user input
                    result.push_back(C);
                }
            }
        }
    }


    return result;
}

void Pandemic_Map::load_All_Archived()
{
    for (auto& File : ArchivedData_files)
    {
        Epidemic_Map Epidemic;
        Epidemic.load_Archived_OutbreakData(File);
        Archived_Data.emplace_back(Epidemic);
    }
}
void Pandemic_Map::load_All_DailyReports()
{
    for (auto& File : DailyReport_files)
    {
        Epidemic_Map Epidemic;
        Epidemic.load_Daily_OutbreakData(File);
        Daily_Reports.emplace_back(Epidemic);
    }
}
void Pandemic_Map::load_All_TimeSeries()
{
    for (auto& File : TimeSeries_files)
    {
        Time_Series.emplace_back(Daily_Update(File));
    }
}


Daily_Update::Daily_Update(std::string _file)
    :
    File (CSV_Parser(_file))
{

    for(auto& UP: File.Parsed_data)
    {
        Location_t Area;
        Area.Region = UP[0];
        Area.Province = UP[1];
        try{
            Area.Latitude = std::stof(UP[2]);
            Area.Longitude = std::stof(UP[3]);
        }catch (...){
            Area.Latitude = 0;
            Area.Longitude = 0;
        }
        try {
            Location_Total[Area.Province] += std::stoi(UP.back());
        }
        catch (...)
        {
            Location_Total[Area.Province] += 0;
        }
        Places.emplace_back(Area);
    }

   

 }













std::ostream& operator <<(std::ostream& _str, Location_t _place)
{
    _str << "Location:  " << _place.Province << " - " << _place.Region <<"\n";
    _str << "Latitude:  " << _place.Latitude << " - ";
    _str << "Longitude: " << _place.Longitude << "\n";
    return _str;
}
std::ostream& operator <<(std::ostream& _str, Date_t _time)
{
    _str << _time.Year << "-" << _time.Month << "-" << _time.Day << " [T]: " << _time.Time.Hours << ":" << _time.Time.Minute << ":" << _time.Time.Seconds <<"\n";
    return _str;
}
std::ostream& operator <<(std::ostream& _str, Outbreak_info _time)
{

   _str <<  _time.Place   << "\n";
   _str << "Confirmed : " << _time.Confirmed  << "\n";
   _str << "Deaths    : " << _time.Deaths     << "\n";
   _str << "Recovered : " << _time.Recovered  << "\n";
   _str << "Date_Time : " << _time.Date_Time  << "\n";
    return _str;
}











/* =============================================================================================================
                                  TRASH
   =============================================================================================================
             //  "Latitude",
          //  "Longitude"
            // Long,
           //  Lat))
            // Row_data.Place.Longitude = std::stof(Long);
            // Row_data.Place.Latitude = std::stof(Lat);

   // Print("Error with Time String");
  // Print("Error with Hour string");           // Print("Error with Day String");

         //Print("OUTBREAK INFORMATION: \n" << O);
         //Print("Local Cases: " << O.Confirmed);
         //Print("      Recovered: " << O.Recovered);
         //Print("      Deaths: " << O.Deaths);


        /* Checks the User Configuration file to see if there are Outbreaks in their Area 
        //if (Outbreak_Map.find(_location) != Outbreak_Map.end())
        //{// If there are outbreaks Report Each one 
        //    Print("\n");
        //    ColorPrint(CON_Red, "~WARNING~ Outbreak Detected in your area: ");
        //    for (auto& O : Outbreak_Map[Config.User_Location.Province])
        //    {
        //        Print("OUTBREAK INFORMATION: \n" << O);
        //        Print("Local Cases: " << O.Confirmed);
        //        Print("      Recovered: " << O.Recovered);
        //        Print("      Deaths: " << O.Deaths);
        //    }
        //    return true;
        //}

        //for (auto& O : Outbreak_Map)
        //{
        //    for (auto& C : O.second)
        //    {
        //        if (C.Place.Region == _location)
        //        {
        //            Print("Area Found");
        //        }
        //    }
        //}

                Print("Map Ascii");
                for (auto& L : UcaseRegion) Print((int)L);
                Print("User Ascii");
                for (auto& L : UcaseInput) Print((int)L);


//    Print("OUTBREAK INFORMATION: \n" << O);
// Print("Local Cases: " << O.Confirmed);
// Print("      Recovered: " << O.Recovered);
// Print("      Deaths: " << O.Deaths);


// _str << "Year   :" << _time.Year << "\n";
// _str << "Month  :" << _time.Month << "\n";
// _str << "Day    :" << _time.Day << "\n";
// _str << "Hours  :" << _time.Time.Hours << "\n";
// _str << "Minute :" << _time.Time.Minute << "\n";
// _str << "Seconds:" << _time.Time.Seconds << "\n";










//  "Province / State",
//  "Country / Region",
//  "Last Update"     ,
//  "Confirmed"	      ,
//  "Deaths"          ,
//  "Recovered"	      ,
//  "Latitude"        ,
//  "Longitude"       ,

//Province / State	Country / Region	Last Update	Confirmed	Deaths	Recovered	Latitude	Longitude




        try
        {
            io::CSVReader<6, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> in(_filename);// "Cases.csv");
            in.read_header
            (
                io::ignore_extra_column,/// io::ignore_no_column , ///
                "Province/State",
                "Country/Region",
                "Last Update",
                "Confirmed",
                "Deaths",
                "Recovered",
                "Longitude",
                "Latitude"

            );

            Outbreak_info Row_data;
            std::string DateTimeIN;
            std::string Long, Lat;
            while (
                in.read_row(
                    Row_data.Place.Province,
                    Row_data.Place.Region,
                    DateTimeIN,
                    Row_data.Confirmed,
                    Row_data.Deaths,
                    Row_data.Recovered))
            {

                Row_data.Date_Time = Date_t::parse_DateTimeString(DateTimeIN);
                Row_data.Place.Latitude = 0;
                Row_data.Place.Longitude = 0;

                Outbreak_List.push_back(Row_data);

                Total_Deaths += Row_data.Deaths;
                Total_Infected += Row_data.Confirmed;
                Total_Recovered += Row_data.Recovered;

                Outbreak_Map[Row_data.Place.Province].push_back(Row_data);
            }


        }
        catch (...)
        {
            try
            {

*/