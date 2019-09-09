#include <Arduino.h>
#include "sd/SD.hpp"
#include "can/Canmsg.hpp"
#include "can/CanUtility.hpp"
#include "utilities/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"
#include "display/ILI9341.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/HomePage.hpp"
#include "display/screenBuffer.hpp"
#include "utilities/utilities.hpp"

#define TEST
#include "utilities/testmessages.hpp"

using namespace utilities; // für scom

ILI9341 display {PC9, PC8, PA10, PA8, PB5, true};
DisplayPageManager pageManager {};

void setup() {
    Serial.begin(115200);
    scom.workWith(Serial); // scom Hardwareserial zuweisen
    scom.setDebugMode(true); // Debugmodus einschalten
  
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

/*
//für Serielle Ausgaben zum testen
int timeCounter1ms = 0;
void HAL_SYSTICK_Callback(void)
{
    if(timeCounter1ms > 5000)
    {
        timeCounter1ms = 0;
    }    
    timeCounter1ms++;
}
*/

void loop() {
    loopTaster();
    pageManager.loop();
    loopScreenBuffer();
<<<<<<< HEAD
    
=======
    #ifdef TEST
    sendTestmessages();
    #endif //TEST
>>>>>>> ff890565bece3d54b17469c47cf8bff1c8677e26
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
