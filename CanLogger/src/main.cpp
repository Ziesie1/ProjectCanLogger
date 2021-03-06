#include <Arduino.h>
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"
#include "utilities/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"
#include "display/ILI9341.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/HomePage.hpp"
#include "display/screenBuffer.hpp"
#include "utilities/utilities.hpp"

// #define TEST_SENDANDRECEIVE_MESSAGES
// #include "utilities/testmessages.hpp"

using namespace utilities; // für scom

ILI9341 display {PC9, PC8, PA10, PA8, PB5, true};
DisplayPageManager pageManager {};

void setup() {
    Serial.begin(115200);
    scom.workWith(Serial); // scom Hardwareserial zuweisen
    //scom.setDebugMode(true); // Debugmodus einschalten
  
    HAL_Init();
    SystemClock_Config();
    initEncoder();
    initTaster();
    screenBufferInit();
    display.init();

    if((CanUtility_Init(CAN_500_KBIT) != HAL_OK))
    {
        scom.printError("Konnte CanUtility nicht Initialisieren.\nDas Programm wird angehalten!");
        while(1){}
    }
 
    pageManager.openNewPage(new HomePage{display}); // Startseite setzen
  
    scom << "CanLogger ist Initialisiert" << endz;
}

void loop() {
    loopTaster();
    pageManager.loop();
    loopScreenBuffer();

    #ifdef TEST_SENDANDRECEIVE_MESSAGES
    sendTestmessages();
    #endif //TEST
}

void serialEvent() 
{
    while (Serial.available()) 
    {
        char inChar = (char)Serial.read();
        /*
            Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
        */
        scom << "Charakter recieved:" << inChar << endz;
    }
}
