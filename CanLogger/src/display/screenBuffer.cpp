#include "display/screenBuffer.hpp"
#include "can/Canmsg.hpp"
#include "can/CanUtility.hpp"
#include <Arduino.h>
#include "utilities/SerialCommunication.hpp"

//extern:
int screenBufferFillLevel = 0;
int screenBufferUserViewFillLevel = 0;
Canmsg** screenBuffer = nullptr;
Canmsg** screenBufferUserView = nullptr;
bool updateUserView = false;

//intern:
bool CanUtility_CanRecieveActive_lastStatus = false;

/* 
    sets up the front- and the backendbuffer
*/
void screenBufferInit(void)
{
  screenBuffer = new Canmsg*[SCREEN_BUFFER_SIZE];
  screenBufferUserView = new Canmsg*[SCREEN_BUFFER_SIZE];
  for(int i=0; i<SCREEN_BUFFER_SIZE; i++)
  {
    screenBuffer[i] = nullptr;
    screenBufferUserView[i] = nullptr;
  }
  screenBufferFillLevel = 0;
  screenBufferUserViewFillLevel = 0;
  updateUserView = false;
}

/*
    deletes the front- and the backendbuffer
*/
void screenBufferDeinit(void)
{
  if(screenBuffer)
  {
    for(int i=0; i<SCREEN_BUFFER_SIZE; i++)
    {
      if(screenBuffer[i])
      {
        delete screenBuffer[i];
      }
    }
    delete[] screenBuffer;
    screenBuffer = nullptr;
    screenBufferFillLevel = 0;
  }

  if(screenBufferUserView)
  {
    for(int i=0; i<SCREEN_BUFFER_SIZE; i++)
    {
      if(screenBufferUserView[i])
      {
        delete screenBufferUserView[i];
      }
    }
    delete[] screenBufferUserView;
    screenBufferUserView = nullptr;
    screenBufferUserViewFillLevel = 0;
  }

  updateUserView = false;
}

/* 
    erases the content of the front- and backendbuffer
*/
void clearScreenBuffers(void)
{
  screenBufferDeinit();
  screenBufferInit();
}

/* 
    function that prints the content of the screenBuffer via the Serial Port
*/
void printScreenBufferSerial(void)
{
  String s = "Backendbuffer Fuellstand: ";
  s += String(screenBufferFillLevel, DEC);
  s += "/";
  s += String(SCREEN_BUFFER_SIZE, DEC);
  utilities::scom.printDebug(s);
  for(int i=0; i<screenBufferFillLevel; i++)
  {
    if(screenBuffer[i])
    {
      s = "Nachricht ";
      if((i+1)<10)
      {
        s += " ";
      }
      s += String(i+1, DEC);
      s += "/";
      s += String(screenBufferFillLevel, DEC);
      s += ": ";
      s += static_cast<String>(*screenBuffer[i]);
      utilities::scom.printDebug(s);
    }
  }
  utilities::scom.printDebug("");
}

/* 
    function that prints the content of the screenBufferUserView via the Serial Port
*/
void printScreenBufferUserViewSerial(void)
{
  String s = "Ausgabebuffer Fuellstand: ";
  s += String(screenBufferUserViewFillLevel, DEC);
  s += "/";
  s += String(SCREEN_BUFFER_SIZE, DEC);
  utilities::scom.printDebug(s);
  for(int i=0; i<screenBufferUserViewFillLevel; i++)
  {
    if(screenBufferUserView[i])
    {
      s = "Nachricht ";
      if((i+1)<10)
      {
        s += " ";
      }
      s += String(i+1, DEC);
      s += "/";
      s += String(screenBufferUserViewFillLevel, DEC);
      s += ": ";
      s += static_cast<String>(*screenBufferUserView[i]);
      utilities::scom.printDebug(s);
    }
  }
  utilities::scom.printDebug("");
}

/* 
    inserts the surpassed message at the surpassed position
    Input:  msg - reference to the message that should be inserted
            pos - position the message should be Placed
                  note that if the buffer is allready filled, the last message will be discarded
                  possible values: 0 - screenBufferFillLevel
*/
void insertMessageHere(Canmsg const& msg, int pos)
{
  if((pos < SCREEN_BUFFER_SIZE) && (pos <= screenBufferFillLevel))
  {
    if(screenBufferFillLevel >= SCREEN_BUFFER_SIZE)
    {
      screenBufferFillLevel = SCREEN_BUFFER_SIZE-1;
    }
    if(screenBuffer[screenBufferFillLevel])
    {
      delete screenBuffer[screenBufferFillLevel];
    }
    screenBuffer[screenBufferFillLevel] = new Canmsg{msg};
    for(int i=screenBufferFillLevel; i>pos; i--)
    {
      Canmsg* temp = screenBuffer[i]; 
      screenBuffer[i] = screenBuffer[i-1];
      screenBuffer[i-1] = temp;
      temp = nullptr;
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
void sortCanMessageIntoBuffer(Canmsg const& msg)
{
  bool messageInserted = false;
  for(int i=0; i<screenBufferFillLevel && !messageInserted; i++)
  {
    if(!msg.GetIsExtIdentifier())
    {
      if(!(*screenBuffer[i]).GetIsExtIdentifier())
      {
        if(msg.GetStdIdentifier() == (*screenBuffer[i]).GetStdIdentifier())
        {
          if(screenBuffer[i])
          {
            delete screenBuffer[i];
          }
          screenBuffer[i] = new Canmsg{msg};
          messageInserted = true;
        }
        else if(msg.GetStdIdentifier() < (*screenBuffer[i]).GetStdIdentifier())
        {
          insertMessageHere(msg, i);
          messageInserted = true;
        }
      }
      else
      {
        insertMessageHere(msg, i);
        messageInserted = true;
      }
    }
    else
    {
      if((*screenBuffer[i]).GetIsExtIdentifier())
      {
        if(msg.GetStdIdentifier() == (*screenBuffer[i]).GetStdIdentifier())
        {
          if(msg.GetExtIdentifier() == (*screenBuffer[i]).GetExtIdentifier())
          {
            if(screenBuffer[i])
            {
              delete screenBuffer[i];
            }
            screenBuffer[i] = new Canmsg{msg};
            messageInserted = true;
          }
          else if(msg.GetExtIdentifier() < (*screenBuffer[i]).GetExtIdentifier())
          {
            insertMessageHere(msg, i);
            messageInserted = true;
          }
        }
        else if(msg.GetStdIdentifier() < (*screenBuffer[i]).GetStdIdentifier())
        {
          insertMessageHere(msg, i);
          messageInserted = true;
        }
      }
    }
  }
  if((screenBufferFillLevel < SCREEN_BUFFER_SIZE) && !messageInserted)
  {
    if(screenBuffer[screenBufferFillLevel])
    {
      delete screenBuffer[screenBufferFillLevel];
    }
    screenBuffer[screenBufferFillLevel] = new Canmsg{msg};
    screenBufferFillLevel++;
  }
}

/* 
    copies the messages from the Backendbuffer to the Frontendbuffer
*/
void makeBufferVisible(void)
{
  for(int i=0; i<screenBufferFillLevel; i++)
  {
    if(!screenBufferUserView[i])
    {
      screenBufferUserView[i] = new Canmsg{*screenBuffer[i]};
    }
    else if(*(screenBufferUserView[i]) != *(screenBuffer[i]))
    {
      *(screenBufferUserView[i]) = *(screenBuffer[i]);
    }
  }
  screenBufferUserViewFillLevel = screenBufferFillLevel;
}

/* 
    function that processes the last message of the "Canmsg_bufferCanRecMessages"
*/
void loopScreenBuffer(void)
{
  if(CanUtility_CanRecieveActive)
  {
    CanUtility_CanRecieveActive_lastStatus = true;

    while(CanUtility_bufferCanRecPointer > 0)
    {
      CanUtility_bufferCanRecPointer--;

      // um die Nachrichten in der Reihenfolge abzuarbeiten, in der sie empfangen wurden
      Canmsg* curMsg = CanUtility_bufferCanRecMessages[0];
      for(int i=0; i<CanUtility_bufferCanRecPointer; i++)
      {
        CanUtility_bufferCanRecMessages[i] = CanUtility_bufferCanRecMessages[i+1];
      }
      CanUtility_bufferCanRecMessages[CanUtility_bufferCanRecPointer] = nullptr;

      //screenBuffer:
      sortCanMessageIntoBuffer(*curMsg);

      //SD-Card:



      //loopback:
      /*
      CanUtility_SendMessage(curMsg);
      */
      
      //Serial:

      String s = "Empfangene Nachricht: ";
      s += static_cast<String>(*curMsg);
      utilities::scom.printDebug(s);
      
      //free space
      delete curMsg;
    }
    if(CanUtility_discardedMessages>0)
    {
      String s = "Anzahl verworfener Nachrichten: ";
      s += String(CanUtility_discardedMessages, DEC);
      utilities::scom.printDebug(s);
    }
  }
  else if(CanUtility_CanRecieveActive_lastStatus)
  {
    CanUtility_CanRecieveActive_lastStatus = false;
    clearScreenBuffers();
  }

  if(updateUserView)
  {
    makeBufferVisible();
  }
}

