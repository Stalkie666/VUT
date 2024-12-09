#include "ESP32_security_system.hpp"

int takeAPhoto(){
    camera_fb_t * fb = NULL;
    // Take Picture with Camera
    fb = esp_camera_fb_get();  
    if(!fb) {
      Serial.println("Camera capture failed");
      return 1;
    }


    pictureNumber = loadPictureNumberFromSD();

    // Path where new picture will be saved in SD Card
    String path = "/picture" + String(pictureNumber) +".jpg";

    fs::FS &fs = SD_MMC; 
    Serial.printf("Picture file name: %s\n", path.c_str());
    
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file in writing mode");
    } 
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.printf("Saved file to path: %s\n", path.c_str());
    }
    file.close();
    esp_camera_fb_return(fb); 

    pictureNumber++;
    savePictureNumberToSD(pictureNumber);

    return 0;
}


