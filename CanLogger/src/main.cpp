#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

using namespace utilities; // für scom

void printScreenBufferSerial(void);
void processCanMessage(void);
void insertMessageHere(Canmsg& msg, int pos);
constexpr int SCREEN_BUFFER_SIZE = 10;
static int screenBufferFillLevel = 0;
static Canmsg* screenBuffer = nullptr;
static Canmsg* screenBufferUserView = nullptr;

void setup() {
  HAL_Init();
  //HAL_InitTick(14);
  SystemClock_Config();
  Serial.begin(115200);
  //scom.workWith(Serial); // scom Hardwareserial zuweisen
  //scom.showDebugMessages(true); // Debugmodus einschalten
  
  //init_SD();

  if((CanUtility_Init() != HAL_OK) || (CanUtility_EnableRecieve() != HAL_OK))
  {
    while(1){}
  }
  screenBuffer = new Canmsg[SCREEN_BUFFER_SIZE];
  screenBufferUserView = new Canmsg[SCREEN_BUFFER_SIZE];
  screenBufferFillLevel = 0;
  
  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");
  
  Serial.println("µC initialized");
}


void loop() {
  //Serial.println(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, 0));
  while(Canmsg_bufferCanRecPointer > 0)
  {
    processCanMessage();
  }
  printScreenBufferSerial();
  delay(5000);
}

/* 
    function that prints the content of the screenBuffer via the Serial Port
*/
void printScreenBufferSerial(void)
{
  Serial.print("Ausgabebuffer Fuellstand: ");
  Serial.print(screenBufferFillLevel);
  Serial.print("/");
  Serial.println(SCREEN_BUFFER_SIZE);
  for(int i=0; i<screenBufferFillLevel; i++)
  {
    Serial.print("Nachricht ");
    if((i+1)<10)
    {
      Serial.print(" ");
    }
    Serial.print(i+1);
    Serial.print("/");
    Serial.print(screenBufferFillLevel);
    Serial.print(": ");
    Serial.println(static_cast<String>(screenBuffer[i]));
  }
  Serial.println("\r\n");
}

/* 
    inserts the surpassed message at the surpassed position
    Input:  msg - reference to the message that should be inserted
            pos - position the message should be Placed
                  note that if the buffer is allready filled, the last message will be discarded
                  possible values: 0 - screenBufferFillLevel
*/
void insertMessageHere(Canmsg& msg, int pos)
{
  if((pos < SCREEN_BUFFER_SIZE) && (pos <= screenBufferFillLevel))
  {
    if(screenBufferFillLevel >= SCREEN_BUFFER_SIZE)
    {
      screenBufferFillLevel = SCREEN_BUFFER_SIZE-1;
    }
    screenBuffer[screenBufferFillLevel] = msg;
    for(int i=screenBufferFillLevel; i>pos; i--)
    {
      Canmsg temp = std::move(screenBuffer[i]); 
      screenBuffer[i] = std::move(screenBuffer[i-1]);
      screenBuffer[i-1] = std::move(temp);
    }
    if(screenBufferFillLevel < SCREEN_BUFFER_SIZE)
    {
      screenBufferFillLevel++;
    }
  }
}

/* 
    function that processes all the pending CAN-messages in the Canmsg_bufferCanRecMessages
*/
void processCanMessage(void)
{
  Canmsg_bufferCanRecPointer--;
  Canmsg curMsg = std::move(Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer]);
  
  //screenBuffer:
  bool messageInserted = false;
  for(int i=0; i<screenBufferFillLevel && !messageInserted; i++)
  {
    if(!curMsg.GetIsExtIdentifier())
    {
      if(!screenBuffer[i].GetIsExtIdentifier())
      {
        if(curMsg.GetStdIdentifier() == screenBuffer[i].GetStdIdentifier())
        {
          screenBuffer[i] = curMsg;
          messageInserted = true;
        }
        else if(curMsg.GetStdIdentifier() < screenBuffer[i].GetStdIdentifier())
        {
          insertMessageHere(curMsg, i);
          messageInserted = true;
        }
      }
      else
      {
        insertMessageHere(curMsg, i);
        messageInserted = true;
      }
    }
    else
    {
      if(screenBuffer[i].GetIsExtIdentifier())
      {
        if(curMsg.GetStdIdentifier() == screenBuffer[i].GetStdIdentifier())
        {
          if(curMsg.GetExtIdentifier() == screenBuffer[i].GetExtIdentifier())
          {
            screenBuffer[i] = curMsg;
            messageInserted = true;
          }
          else if(curMsg.GetExtIdentifier() < screenBuffer[i].GetExtIdentifier())
          {
            insertMessageHere(curMsg, i);
            messageInserted = true;
          }
        }
        else if(curMsg.GetStdIdentifier() < screenBuffer[i].GetStdIdentifier())
        {
          insertMessageHere(curMsg, i);
          messageInserted = true;
        }
      }
    }
  }
  if((screenBufferFillLevel < SCREEN_BUFFER_SIZE) && !messageInserted)
  {
    screenBuffer[screenBufferFillLevel] = curMsg;
    screenBufferFillLevel++;
  }

  //SD-Card:

  //loopback:
  /*
  Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer].Send();
  */

  //Serial:
  /*
  Serial.print("Empfangene Nachricht: ");
  Serial.println(static_cast<String>(Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer]));
  */
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    /*
      Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
    */
    scom << "Charakter recieved:" << inChar << endz;

    Canmsg msg{};
    scom << static_cast<String>(msg) << endz;
  }
  
}



