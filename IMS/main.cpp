/**
 * main.cpp
 * IMS, T2: Logistika
 * Author: Jakub Hamadej, xhamad03, FIT VUT, 28. 11. 2024
 */

#include <iostream>
#include "simlib.h"
#include "convoySettings.hpp"

Histogram travel_time("Travel time",0,1,100);

// TODO: globalni prommene pro zaznam statistik
int CelkovyPocetSevernichKonvoju = 0;
int CelkovyPocetJiznichKonvoju = 0;
int OdbavenyPocetSevernichKonvoju = 0;
int OdbavenyPocetJiznichKonvoju = 0;

// TODO: facilities pro kontrolu/povoleni vstupu do jednotlivych casti kanalu
/**
 * Allows enter parts of canal
 */
Facility PortSaid_Ismaille_Facility;
Facility BitterLake_Suez_Facility;

Facility North_to_South_Facility;
Facility South_to_North_Facility;

double VytvoreneKonvoje = 0;
double OdbaveneKonvoje = 0;

class NorthToSouthRoute : public Process{
    public:
        void Behavior(){
            VytvoreneKonvoje++;
            /**
             * Enter Port Said - Ismaille part of canal
            */
            double start_time = Time;
            Seize(North_to_South_Facility);
            Seize(PortSaid_Ismaille_Facility);
            Wait( Uniform(HOUR * 4, HOUR * 5) );
            /**
             * Exit Port Said - Ismaille part of canal
             * Enter route Ismaille - End of bitter lake
             */
            Release(PortSaid_Ismaille_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 5) );
            /**
             * Enter Bitter Lake - Suez part of canal
             * Allow next convoy from north to enter canal
             */
            Seize(BitterLake_Suez_Facility);
            Release(North_to_South_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 4) );
            /**
             * Exit Suez canal
             */
            Release(BitterLake_Suez_Facility);
            travel_time((Time - start_time)/ 60);
            OdbaveneKonvoje++;
        }
};

class SouthToNorthRoute : public Process{
    public:
        void Behavior(){
            VytvoreneKonvoje++;
            /**
             * Enter Suez - Bitter Lake part
             */
            double start_time = Time;
            Seize(South_to_North_Facility);
            Seize(BitterLake_Suez_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 4) );
            /**
             * Exit Suez - Bitter Lake part
             * Enter Bitter Lake - Ismaille part
             */
            Release(BitterLake_Suez_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 5) );
            /**
             * Enter Ismaille - Port Said part
             * Allow next convoy from south to enter canal
             */
            Seize(PortSaid_Ismaille_Facility);
            Release(South_to_North_Facility);
            Wait( Uniform(HOUR * 4,HOUR * 5) );
            /**
             * Exit canal at Port Said
             */
            Release(PortSaid_Ismaille_Facility);
            travel_time((Time - start_time)/ 60);
            OdbaveneKonvoje++;
        }
};

//TODO: generator konvoju ze severu
class GenerateConvoyFromNorth : public Event{
  public:
    void Behavior(){
        (new NorthToSouthRoute)->Activate();
        Activate(Time + Exponential(HOUR * 12));
    }  
};
//TODO: generator konvoju z jihu
class GenerateConvoyFromSouth : public Event{
  public:
    void Behavior(){
        (new SouthToNorthRoute)->Activate();
        Activate(Time + Exponential(HOUR * 24));
    }  
};


// TODO: nejaka funkce na vypis statistik


int main(){

    Init(0,YEAR);
    (new GenerateConvoyFromNorth)->Activate();
    (new GenerateConvoyFromSouth)->Activate();
    Run();
    travel_time.Output();
    std::cout << "Vytvorene konvoje: " << VytvoreneKonvoje << std::endl;
    std::cout << "Odbavene  konvoje: " << OdbaveneKonvoje << std::endl;

    return 0;
}