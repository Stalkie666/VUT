/**
 * main.cpp
 * IMS, T2: Logistika
 * Author: Jakub Hamadej, xhamad03, FIT VUT, 28. 11. 2024
 */

#include <iostream>
#include <ctime>
#include "simlib.h"
#include "convoySettings.hpp"

/**
 * Statistics global variables
 */
double VytvoreneKonvoje = 0;
double OdbaveneKonvoje = 0;
double NehodyNaKanalu = 0;

/**
 * Allows enter parts of canal
 */
Facility PortSaid_Ismaille_Facility;
Facility BitterLake_Suez_Facility;

Facility North_to_South_Ismaille_Facility;
Facility North_to_South_BitterLake_Facility;

Facility South_to_North_Ismaille_Facility;
Facility South_to_North_BitterLake_Facility;


/**
 * Those processes simulates convoy going throw canal
 */
class NorthToSouthRoute : public Process{
    public:
        void Behavior(){
            Priority = 1;
            VytvoreneKonvoje++;
            /**
             * Enter Port Said - Ismaille part of canal
            */
            Seize(North_to_South_Ismaille_Facility);
            Seize(PortSaid_Ismaille_Facility);
            Wait( Uniform(HOUR * 4, HOUR * 5) );

            Priority = 0;

            /**
             * Exit Port Said - Ismaille part of canal
             * Enter route Ismaille - Start of bitter lake
             */
            Release(PortSaid_Ismaille_Facility);
            Wait( Uniform(HOUR * 2, HOUR * 3) );

            /**
             * Start of Bitter lake - End of Bitter Lake
            */
            Seize(North_to_South_BitterLake_Facility);
            Release(North_to_South_Ismaille_Facility);
            Wait( Uniform(HOUR * 1, HOUR * 2) );

            /**
             * Enter Bitter Lake - Suez part of canal
             * Allow next convoy from north to enter canal
             */
            Seize(BitterLake_Suez_Facility);
            Release(North_to_South_BitterLake_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 4) );
            /**
             * Exit Suez canal
             */
            Release(BitterLake_Suez_Facility);
            OdbaveneKonvoje++;
        }
};

class SouthToNorthRoute : public Process{
    public:
        void Behavior(){
            Priority = 1;
            VytvoreneKonvoje++;
            /**
             * Enter Suez - Bitter Lake part
             */
            Seize(South_to_North_BitterLake_Facility);
            Seize(BitterLake_Suez_Facility);
            Wait( Uniform(HOUR * 3, HOUR * 4) );

            Priority = 0;
            /**
             * Exit Suez - Bitter Lake part
             * Enter Bitter Lake - End of Bitter Lake part
             */
            Release(BitterLake_Suez_Facility);
            Wait( Uniform(HOUR * 1, HOUR * 2) );



            /**
             * End of Bitter Lake - Ismaille
             */
            Seize(South_to_North_Ismaille_Facility);
            Release(South_to_North_BitterLake_Facility);
            Wait( Uniform(HOUR * 2, HOUR * 3) );


            /**
             * Enter Ismaille - Port Said part
             * Allow next convoy from south to enter canal
             */
            Seize(PortSaid_Ismaille_Facility);
            Release(South_to_North_Ismaille_Facility);
            Wait( Uniform(HOUR * 4,HOUR * 5) );
            /**
             * Exit canal at Port Said
             */
            Release(PortSaid_Ismaille_Facility);
            OdbaveneKonvoje++;
        }
};

/**
 * This process simulate Ever Given ship, which blocked Suez for 6 days
 */
class EverGivenAccident : public Process{
    public:
        EverGivenAccident(){
            Priority = 100;
        }
        void Behavior(){
            NehodyNaKanalu++;
            Seize(BitterLake_Suez_Facility);
            Wait(EVER_GIVEN_ACCIDENTS_RECOVER_TIME);
            Release(BitterLake_Suez_Facility);
        }
};

/**
 * Convoys generators, accidents generator
 */
class GenerateConvoyFromNorth : public Event{
  public:
    void Behavior(){
        (new NorthToSouthRoute)->Activate();
        Activate(Time + (NORTH_CONVOYS_PER_DAY));
    }  
};

class GenerateConvoyFromSouth : public Event{
  public:
    void Behavior(){
        (new SouthToNorthRoute)->Activate();
        Activate(Time + (SOUTH_CONVOYS_PER_DAY));
    }  
};

class GenerateSpecialConvoysFromNorth : public Event{
    public:
        void Behavior(){
            (new NorthToSouthRoute)->Activate();
            Activate(Time + Exponential(SPECIAL_CONVOYS_FROM_NORTH_PER_MONTH));
        }
};
class GenerateSpecialConvoysFromSouth : public Event{
    public:
        void Behavior(){
            (new SouthToNorthRoute)->Activate();
            Activate(Time + Exponential(SPECIAL_CONVOYS_FROM_SOUTH_PER_MONTH));
        }
};

class GenerateAccidentInCanal : public Event{
    public:
        void Behavior(){
            (new EverGivenAccident)->Activate();
            Activate(Time + Exponential(EVER_GIVEN_ACCIDENTS_PER_MONTH));
        }
};


/**
 * Print statistic gathered during simulations
 */
void PrintStatistics(){
    std::cout << "Vytvorene konvoje: " << VytvoreneKonvoje << std::endl;
    std::cout << "Odbavene  konvoje: " << OdbaveneKonvoje << std::endl;
    std::cout << (((VytvoreneKonvoje - OdbaveneKonvoje) >= 10) ? 
                        "Kanal je na sve kapacite" : 
                        "Kanal je stale funkcni") <<  std::endl;
#if ALLOW_ACCIDENTS
    std::cout << "Nehod na kanalu:   " << NehodyNaKanalu << std::endl;
#endif
}


int main(){
    RandomSeed(time(NULL));
    Init(0,SIMULATION_RUN_TIME);
    (new GenerateConvoyFromNorth)->Activate();
    (new GenerateConvoyFromSouth)->Activate();
#if ALLOW_ACCIDENTS
    (new GenerateAccidentInCanal)->Activate();
#endif
#if ALLOW_SPECIAL_CONVOYS
    (new GenerateSpecialConvoysFromNorth)->Activate();
    (new GenerateSpecialConvoysFromSouth)->Activate();
#endif

    Run();
    
    PrintStatistics();
    return 0;
}