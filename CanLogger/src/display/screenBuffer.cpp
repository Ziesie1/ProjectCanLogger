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

//intern:
bool updateUserView = false;
bool somethingChanged = false;
bool whatChanged[SCREEN_BUFFER_SIZE];
bool screenBufferInitialized = false;

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
  somethingChanged = false;
  for(int i=0; i<SCREEN_BUFFER_SIZE; i++)
  {
    whatChanged[i] = false;
  }

  screenBufferInitialized = true;
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
  somethingChanged = false;
  for(int i=0; i<SCREEN_BUFFER_SIZE; i++)
  {
    whatChanged[i] = false;
  }

  screenBufferInitialized = false;
}

/* 
    erases the content of the front- and backendbuffer
*/
void screenBuffer_clearScreenBuffer(void)
{
  screenBufferDeinit();
  screenBufferInit();
}

/* 
    function that prints the content of the screenBuffer via the Serial Port
*/
void printScreenBufferSerial(void)
{
  if(screenBufferInitialized)
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
}

/* 
    function that prints the content of the screenBufferUserView via the Serial Port
*/
void printScreenBufferUserViewSerial(void)
{
  if(screenBufferInitialized)
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
  if((pos < SCREEN_BUFFER_SIZE) && (pos <= screenBufferFillLevel) && screenBufferInitialized)
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
  if(screenBufferInitialized)
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
}

/* 
    copies the messages from the Backendbuffer to the Frontendbuffer
*/
void makeBufferVisible(void)
{
  if(screenBufferInitialized)
  {
    for(int i=0; i<screenBufferFillLevel; i++)
    {
      if(!screenBufferUserView[i])
      {
        screenBufferUserView[i] = new Canmsg{*screenBuffer[i]};
        somethingChanged = true;
        whatChanged[i] = true;
      }
      else if(*(screenBufferUserView[i]) != *(screenBuffer[i]))
      {
        *(screenBufferUserView[i]) = *(screenBuffer[i]);
        somethingChanged = true;
        whatChanged[i] = true;
      }
    }
    screenBufferUserViewFillLevel = screenBufferFillLevel;
  }
}

bool screenBuffer_hasSomethingChanged(void)
{
  if(somethingChanged)
  {
    somethingChanged = false;
    return true;
  }
  else
  {
    return false;
  }
}

bool screenBuffer_hasThisMessageChanged(int pos)
{
  if((pos >= 0) && (pos<SCREEN_BUFFER_SIZE))
  {
    if(whatChanged[pos])
    {
      whatChanged[pos] = false;
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

/* 
    function that processes the last message of the "Canmsg_bufferCanRecMessages"
*/
void loopScreenBuffer(void)
{
  if(CanUtility_isRecieveActive() && screenBufferInitialized)
  {
    while(CanUtility_getbufferCanRecMessagesFillLevel() > 0)
    {
      //get message recieved first
      Canmsg* curMsg = CanUtility_readFirstMessageFromBuffer();
      
      if(curMsg)
      {
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
      
        //free space:
        delete curMsg;
      }
    }
    
    if(CanUtility_whereNewMessagesDiscarded())
    {
      String s = "Anzahl verworfener Nachrichten: ";
      s += String(CanUtility_howManyMessagesWhereDiscarded(), DEC);
      utilities::scom.printDebug(s);
    }
    
    if(CanUtility_hasFiFoOverflowOccured())
    {
      utilities::scom.printDebug("FIFO ueberfuellt");
      HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
    }
  }

  if(updateUserView && screenBufferInitialized)
  {
    makeBufferVisible();
  }
}

void screenBuffer_enableUpdate(void)
{
  if(screenBufferInitialized)
  {
    updateUserView = true;
  }
}

void screenBuffer_disableUpdate(void)
{
  updateUserView = false;
}

Canmsg const* screenBuffer_getMessageAtPosition(int pos)
{
  if(screenBufferInitialized && (pos >= 0) && (pos < screenBufferUserViewFillLevel))
  {
    return screenBufferUserView[pos];
  }
  else
  {
    return nullptr;
  }
}

int screenBuffer_getFillLevel(void)
{
  return screenBufferUserViewFillLevel;
}
