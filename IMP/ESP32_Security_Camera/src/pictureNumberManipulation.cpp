#include "ESP32_security_system.hpp"

int loadPictureNumberFromSD(){
    File file = SD_MMC.open("/pictureNumber.txt", FILE_READ);
    if (!file) {
        Serial.println("No pictureNumber file found, starting from 0");
        return 0;
    }
    String content = file.readStringUntil('\n');
    file.close();
    int pictureNumber = content.toInt();
    Serial.printf("Loaded picture number: %d\n", pictureNumber);
    return pictureNumber;
}


void savePictureNumberToSD(int pictureNumber){
    File file = SD_MMC.open("/pictureNumber.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open pictureNumber file for writing");
        return;
    }
    file.print(pictureNumber);
    file.close();
    Serial.printf("Picture number %d saved to SD card\n", pictureNumber);
}