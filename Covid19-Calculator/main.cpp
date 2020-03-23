#include <iostream> 
#include <string> 
#include <string>
#include <iostream>
#include <filesystem>

#include"Covid19calc.h"
 
namespace fs = std::filesystem;

 //TODO: Find a place to put this shit 
std::mutex DEBUGMutex;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
Configuration Config("Config.txt");


_static Error_t ErrorHandler::CurrentError;
std::string ErrorString[] =
{
    "No Errors",
    "File Not Found",
    "To many Colmns in CSV file"
};
#include"FileHandling.h"

#include"JsonParser.h"



// https://github.com/Tyyppi77/imgui_sdl
// https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.1.10.pdf
// http://fabiensanglard.net/shadowmapping/index.php 
//https://www.ncbi.nlm.nih.gov/nuccore/MT072688

#include"Common.h"
#include"Graphics/Application.h"


class Graph_Widget
    : public Widget_base
{
public:
    Graph_Widget(Vec2 _topleft, Vec2 _size, Vec2 _xRange, Vec2 _yRange)
        :
        Widget_base(_topleft, _size)
    {
        Xcoef = (_xRange.x - _xRange.y)/ _size.x;
        Ycoef =( _yRange.x - _yRange.y)/ _size.y;
    }
    void add_Group(Vec3 _color) 
    {
        Colors.push_back(_color);
        Values.push_back(std::vector<float>());
    }

    void add_Value(uint32_t _group, float _val)
    {
        Values[_group] .push_back(_val);
    }

    float Xcoef{ 0.0f }, Ycoef{ 0.0f };

    std::vector<std::vector<float>> Values;
    std::vector<Vec3> Colors;
    virtual void OnUpdate(SDL_Event &_msg) {}
  //  virtual void OnRender() { Print("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"); }
    virtual void OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState) {}   // SDL_MOUSEMOTION/
    virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks) {} // SDL_MOUSEBUTTONDOWN
    virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state) {} //SDL_MOUSEBUTTONUP
    virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat) {} //SDL_KEYDOWN
    virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod) {}   //SDL_KEYUP

    virtual void OnRender() override
    {
        SDL_Rect R =
        {
             (int)Area.Position.x,
             (int)Area.Position.y,
             (int)Area.Size.x,
             (int)Area.Size.y
        };


        SDL_SetRenderDrawColor(Context, 250, 250, 250, 255);


        SDL_RenderFillRect(Context, &R);


        for_loop(C, Colors.size())
        {
            SDL_SetRenderDrawColor(Context, Colors[C].x, Colors[C].y, Colors[C].z, 255);
            uint32_t
                x1 = R.x,
                y1 = R.h;// -*Values[C].begin();

            float Xcoef = g_Width() / Values[C].size();
            for (auto& V : Values[C])
            {
                uint32_t
                    px1 = x1,
                    py1 = y1;

                x1 += Xcoef;
                y1 = g_Height() - V;

                SDL_RenderDrawLine(Renderer::get().g_Context(), px1, py1, x1, y1);

            }
        }
            // Render the Children of the Window
        for (auto & C : Children)
        {// In Update, Children first, In Render We draw last so they remain ontop
            C->Render();
        }

        SDL_SetRenderDrawColor(Context, DEFAULT_DRAW_COLOR);

    }
};


class MyButton
    :
    public Button
{
public:
    MyButton(Vec2 _pos, Vec2 _size)
        :Button(_pos, _size)
    {}

    //virtual void OnUpdate(SDL_Event& _msg) override;
   // virtual void OnRender() override;
    virtual void OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState) override 
    {
        Print("Mouse over Button");
    }
    virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks) override
    {// SDL_MOUSEBUTTONDOWN
        Print("Mouse click Button");

    } 
    virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state) override
    { //SDL_MOUSEBUTTONUP
        Print("Mouse up Button");

    }
    virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat) override 
    { //SDL_KEYDOWN
        Print("keydown Button");

    }
    virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod) override
    {  //SDL_KEYUP
        Print("Keyup Mouse click Button");
    } 
};
class MyApp:
    public Application
{

    Pandemic_Map Total_Pandemic;



    virtual void OnCreate() override
    {
        MakeWindow(1280, 960, "Test2");
 
        Graph_Widget *Graph = new Graph_Widget({ 10,10 }, { 800,500 }, { 0, 100 }, { 0,100 });
        Graph->add_Group({ 255,0,0 });
        Graph->add_Group({ 0,255,0 });
        Graph->add_Group({ 0,0,255 });

        Graph->add_Value(0,100);

        for_loop(i, 100)
        {
            Graph->add_Value(0, rand() % (i + 1));
        }
        for_loop(i, 100)
        {
            Graph->add_Value(1, rand() % (i * 2 + 1));
        }      
    
        Total_Pandemic.Retrieve_All_Filenames();
        Total_Pandemic.load_All_Archived();
        Total_Pandemic.load_All_DailyReports();
        for (auto& V : Total_Pandemic.Daily_Reports.back().Outbreak_Map)
        {
            Graph->add_Value(1, V.second.back().Deaths);
        }

        UserInterface = GUI("TestGUI");

        UserInterface.Add(Graph);
    }
    virtual void OnMouseMove(Vec2 _position, Vec2 _relative, uint32_t _buttonState) override 
    { // SDL_MOUSEMOTION/
        Print(_position);
    }
    virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks)override
    {  // SDL_MOUSEBUTTONDOWN
        Print("Click");
    }
    virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state)override
    {  //SDL_MOUSEBUTTONUP
        Print("MouseUp");
    }
    virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat)override
    {   //SDL_KEYDOWN
        Print("Key Down");
    }
    virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod) override
    {   //SDL_KEYUP
        Print("Key Up");
    }

    virtual void OnUpdate() {}
    virtual void OnRender() {}

};



/* =========================================================================================================================================
       Program for Loading John Hopkins Git Repository of Covid-19 data from the start of the outbreak to current day
       Information is matched to a Region/Province and is being made searchable 
       Various calculations are also going to be performed in an attempt to make better predictions and Alert the user when Outbreaks are 
       Near them as well as give an estimate of how long before one gets to them.
   =========================================================================================================================================*/
int main()
{
    MyApp App;
    App.Start();
    App.Run();
    App.End();
 //
 //Window MainWin(640, 480, "Main Window");
 //
 //Json JSON_FileTest("Test.json");
 //

/// Pandemic_Map Total_Pandemic;
/// Total_Pandemic.Retrieve_All_Filenames();
/// Total_Pandemic.load_All_Archived();
/// Total_Pandemic.load_All_DailyReports();
/// 
/// //TODO: Stopped here on 3/17/2020 Need some sort of proper encapslation for the Recover/Death/Confirmed Ratios to make that data clean and searchable then I can contain that inside pandemic map
/// Total_Pandemic.load_All_TimeSeries();
/// 
/// 
/// for (auto &A : Total_Pandemic.Archived_Data)
/// {
///     A.display_Data();
/// }
/// 
/// /* Simple Test run for Searching the Data using the Old Stable Archived Data */
/// bool Running{ true };
/// while (Running)
/// {
///     std::string str;
///     std::cout << "Please enter your Location: \n";
///     std::getline(std::cin, str);
///     if (str == "end") Running = false;
///     std::cout << "Outbreak Information for " << str << "\n";
/// 
///     if (Total_Pandemic.Archived_Data.back().is_Local_Outbreaks(str))
///     {
///         for (auto& P : Total_Pandemic.Archived_Data.back().search_Place(str))
///         {
///             Print(P);
///         }
///     }
///     else
///     {
///         Print("No Outbreaks found. Either check spelling of entered location or area is currently clear ");
///     }
/// }
    return 0;
}










// COVID-19 coronavirus spike holds infectivity details
// https://newsroom.uw.edu/news/covid-19-coronavirus-spike-holds-infectivity-details



/* =============================================================================================================
                                  TRASH
   =============================================================================================================
 
 
 //for (auto& F : Filenames)
//{
//    AllMap.push_back(Epidemic_Map());
//    AllMap.back().load_Archived_OutbreakData(F);
//}
//    Map.load_Archived_OutbreakData(Filenames[0]);        //who_covid_19_situation_reports/who_covid_19_sit_rep_time_series/who_covid_19_sit_rep_time_series.csv"); //Cases.csv"); //who_covid_19_sit_rep_time_series.csv
//    Map.display_Data();
//std::vector<std::string> Filenames;
//std::string path = "COVID-19/archived_data/archived_daily_case_updates/";
//for (const auto & entry : fs::directory_iterator(path))
//{
//    auto P = fs::path(entry.path()).extension();
//    if (entry.path().extension() == ".csv")
//    {
//        Filenames.push_back(entry.path().string());
//    }
//}

 
 
 
 
 // /COVID-19/who_covid_19_situation_reports/who_covid_19_sit_rep_time_series/who_covid_19_sit_rep_time_series.csv
//REPO
//https://github.com/CSSEGISandData/COVID-19.git

   //#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif

//#include<iostream>
////using namespace std;
//
//std::string get_current_dir() {
//    char buff[FILENAME_MAX]; //create string buffer to hold path
//    GetCurrentDir(buff, FILENAME_MAX);
//    std::string current_working_dir(buff);
//    return current_working_dir;
//}


//"F:\Program Files\Git\Git-cmd.exe clone https://github.com/CSSEGISandData/COVID-19.git"


// std::string dir = get_current_dir();
// system("cd ..");
// dir = get_current_dir();
// system("git.exe clone o-https://github.com/CSSEGISandData/COVID-19.git");// F: / Program Files / Git / git - cmd.exe clone https ://github.com/CSSEGISandData/COVID-19.git"); // ..git pull origin master");
// Parser Daily("Cases.csv");Cases.csv");//"COVID-19/archived_data/archived_daily_case_updates/02-13-2020_2115.csv"


*/

///36,507,222.016‬

// Current World Mortality
// 4.174494306665629‬ = 44,775,034 deaths in 4 months at the current pace of Italy. 
/// 
/// Ok no problem, just give me a little bit of time today because if this information is intended for a wider audience I want to make sure what I am saying is correct
/// as I have no intention of spreading misinformation or panic amongst the population.Like yourself I just want to ensure people truly know what they are dealing with
/// and the potential dangers because at the moment far to many people are not taking this seriously here in the USA and that sort of thing can have grave consequences
/// as it is important for everyone to do what is needed to be done inorder for us to combat this in any significant way.
/// 
/// 
/// 
/// (I will provide you with sources late after I gather all the information needed so I can present it in a clear fashion
/*

There were some worrying concerns when the world first analyzed the Genome of this virus(something they did in record time I might add and the world should be applauded for) because of the fact this Virus shares many simularities with the surface spikes on that of HIV. 



------------------------
Most antibodies to gp120 don't bind its functional form, the researchers found.
That's why they can't effectively neutralize HIV. “When an antibody binds to something that's quite flexible, it induces or selects shapes that are not necessarily appropriate for function,”
Kwong explains. “In the functional viral spike, gp120 assumes a very specific shape. Most antibodies can't see that particular shape.”

Source: https://www.nih.gov/news-events/nih-research-matters/insights-into-how-hiv-evades-immune-system

-------------------------

Pradhan and colleagues described 4 unusual SARS-CoV-2 Spike protein insertion sequences that are discontinuous in the amino acid sequence but are thought to converge at the RBD. 
Alignments showed that they are not present in any other coronavirus strains but show identity/similarity with sequences in HIV-1 gp120 and Gag,

Source: https://thenativeantigencompany.com/novel-coronavirus-antigens-now-available/

------------------------


As far as that is concerned we are seeing recent studies coming out with very good news on that front over the past couple days.
Contracting the new coronavirus may protect against future reinfection

Source: https://www.medicalnewstoday.com/articles/contracting-the-new-coronavirus-may-protect-against-future-re-infections

*/





/*

As far as that is concerned we are seeing recent studies coming out with very good news on that front over the past couple days.
Contracting the new coronavirus may protect against future reinfection
//https://www.biorxiv.org/content/10.1101/2020.03.13.990226v1.article-info

//https://www.medicalnewstoday.com/articles/contracting-the-new-coronavirus-may-protect-against-future-re-infections


Uncanny similarity of unique inserts in the 2019-nCoV spike protein to HIV-1 gp120
and Gag

/*  finding of 4 unique inserts in the 2019-nCoV, all of which have
identity /similarity to amino acid residues in key structural proteins of HIV-1 is unlikely to be
fortuitous in nature.*/ 
//[ Meaning it seems unlikely this was the result of a natural evolution of viruses. Take that as you will  ]
//https://www.biorxiv.org/content/10.1101/2020.01.30.927871v1.full.pdf

/*
Insights into How HIV Evades Immune System

But most antibodies can't latch onto and neutralize HIV. 
The proteins on the surface of the virus mutate rapidly and change shape continuously. 
They're also covered with immune - evading carbohydrates called glycans.
gp120

Most antibodies to gp120 don't bind its functional form, the researchers found.
That's why they can't effectively neutralize HIV. “When an antibody binds to something that's quite flexible, it induces or selects shapes that are not necessarily appropriate for function,”
Kwong explains. “In the functional viral spike, gp120 assumes a very specific shape. Most antibodies can't see that particular shape.”

Source: https://www.nih.gov/news-events/nih-research-matters/insights-into-how-hiv-evades-immune-system


*/


/*



Many underestimate significantly what exponential growth is. 
They are about to learn the hard way since starting with a single case,
if doubled every day would equate to 1,073,741,824 or roughly 1 billion from a single case doubling every day

Luckily we are not seeing that but we are currently seeing a doubling of around every 4 days in Italy at this point but
even than that would be 1 billion in only 4 months if this is not controlled. At Italy current Mortality which is exceptionally high 
compared to the rest of the world that would be 92,019,674 deaths in a matter of 4 months. Now our odds of seeing 92 million deaths 
are extremely tiny but the fact of the matter is Exponential growth is no joke and nothing to toy with.
What we are seeing is being right on the cusp of something extremely bad happening due to that exponential growth and unless everyone
in the world starts working together to combat it progress and slowing of this growth is difficult.

Current Mortality : 8.57% Mortality

CASES

06, 2 cases
21, 20        .5
22, 79        .5
23, 150       .5
24, 229
25, 322      .5
26, 400
27, 650      .5
28, 888
29, 1128
01, 1694     .5
02, 2036
03, 2502
04, 3089     .5
05, 3858
06, 4636
07, 5883      ..5
08, 7375
09, 9172
10, 10149     .5 last
11, 12462
12, 15113
13, 17660
14, 21157
15, 24747    .5 current cases
16, 27980
17, 31506
18, 35317
19, 41035
20, 47021    .5


DEATHS
 
22,    2     
23,    3     
24,    6-------.5
25,    10    
26,    12    
27,    17------.5
28,    21    
29,    29    
01,    34------.5
02,    52    
03,    79------.5
04,    107   
05,    148-----.5
06,    197   
07,    233-----.5
08,    366   
09,    463-----.5
10,    631   
11,    827   
12,    1016----.5
13,    1266    
14,    1441     
15,    1809     
16,    2158----.5
17,    2503  
18,    2978  
19,    3405  
20,    4032----.5
21
22
23
24     8064
25
26
27
28     16128
29
30
31
1      32256
2
3
4
5      64512
6
7
8
9     129024
10
11
12
13    258,048‬
14
15
16
17    516,096‬
18
19
20
21   1,032,192‬
22
23
24
25  2,064,384‬
26
27
28
29  4,128,768‬
30
 */