#include "ESP32_security_system.hpp"

int pictureNumber = 0;
int currentPictureNumber = 0;

/**
 * @return 1 if failed, 0 if success
 */
int setupSDcard(){
    if(!SD_MMC.begin()){
        Serial.println("SD Card Mount Failed");
        return 1;
    }
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        Serial.println("No SD Card attached");
        return 1;
    }

    pictureNumber = loadPictureNumberFromSD();
    int currentPictureNumber = pictureNumber;

    return 0;
}