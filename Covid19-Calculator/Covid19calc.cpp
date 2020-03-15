#include"Covid19calc.h"


std::ostream& operator <<(std::ostream& _str, Location_t _place)
{
    _str << "Location:  " << _place.Province << " - " << _place.Region <<"\n";
  //  _str << "Region:    " << _place.Region << "\n";
    _str << "Latitude:  " << _place.Latitude << " - ";
    _str << "Longitude: " << _place.Longitude << "\n";
    return _str;
}

std::ostream& operator <<(std::ostream& _str, Date_t _time)
{//TODO: Format this better in the Near Future
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




*/