#include "display/screenBuffer.hpp"
#include "can/CanUtility.hpp"
#include "utilities/SerialCommunication.hpp"
#include "sd/SD.hpp"

int screenBufferFillLevel = 0; //pointer to the last empty element of screenBuffer
int screenBufferUserViewFillLevel = 0; //pointer to the last empty element of screenBufferUserView
Canmsg** screenBuffer = nullptr; 
/*
Buffer to store all incomming messages from CanUtility.hpp
in the concept it is named "Backendbuffer"
*/
Canmsg** screenBufferUserView = nullptr;
/*
Buffer to store all messages that will currently be shown on the display
in the concept it is named "Frontendbuffer"
*/

bool updateUserView = false; //displays if screenBufferUserView will be updated
bool somethingChanged = false;  //signals if something changed in screenBufferUserView
bool whatChanged[SCREEN_BUFFER_SIZE]; //signals if single elements of screenBufferUserView changed
bool screenBufferInitialized = false; //variable to check if the screenbuffers are initialized

/* 
    sets up the front- and the backendbuffer
*/
void screenBufferInit(void)
{
    if(!screenBufferInitialized)
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
    (sadly very unperformant, there sould be better solutions)
    return  true  - buffers erased
            false - error occured
*/
bool screenBuffer_clearScreenBuffer(void)
{
    screenBufferDeinit();
    screenBufferInit();
    return true;
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
    and shifts all the rearward messages by one position
    Input:  msg - reference to the message that should be inserted
            pos - position the message should be placed
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
    and inserts them at there destined position
    Input:  msg - reference to the message that should be sort into the buffer
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
    copies the messages from the Backendbuffer to the Frontendbuffer and marks changes
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

/*
		has a message in userView changed since the last check
		return: true  - something changed (use "screenBuffer_hasThisMessageChanged" to find out what exactly changed)
			      false - nothing changed
*/
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

/*
		check if the message at a specified position in userView changed
		Input:  pos	- position of the message in userView
				          range: 0-(SCREEN_BUFFER_SIZE-1)
		return: true  - this message was Updated
			      false - this message has not been changed
*/
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
    function that processes the first message of the "Canmsg_bufferCanRecMessages"
*/
void loopScreenBuffer(void)
{
    if(CanUtility_isRecieveActive() && screenBufferInitialized)
    {
        if(CanUtility_getbufferCanRecMessagesFillLevel() > 0)
        {
            //get message recieved first
            Canmsg* curMsg = CanUtility_readFirstMessageFromBuffer();
            if(curMsg)
            {
                //screenBuffer:
                sortCanMessageIntoBuffer(*curMsg); // 0 ms

                //SD-Card:
                saveNewCanMessage(*curMsg); // 5-6 ms
        
                //loopback:
                /*
                CanUtility_SendMessage(curMsg);
                */
    
                //Serial:
                String s = "Empfangene Nachricht: ";
                s += static_cast<String>(*curMsg);
                utilities::scom.printDebug(s); // 4 ms
        
                //free space:
                delete curMsg;
            }
        }
    }

    if(updateUserView && screenBufferInitialized)
    {
        makeBufferVisible();
    }
}

/*
		UserView will be updated from now on
		return: true  - will be updated
			      false - error occured
*/
bool screenBuffer_enableUpdate(void)
{
    if(screenBufferInitialized)
    {
        updateUserView = true;
        return true;
    }
    return false;
}

/*
		UserView will not be updated from now on
		return: true  - will not be updated
			      false - error occured
*/
bool screenBuffer_disableUpdate(void)
{
    updateUserView = false;
    return true;
}

/*
    tells if the userView is currently and will be updated
    return: true  - userView is and will be updated
            false - userView is and will not be updated
*/
bool screenBuffer_updateStatus(void)
{
    return updateUserView;
}

/*
		returns the message of the UserView at a specified position
		Input:  msg   - reference the values of the message should be copied
            pos   - Position of the Message in UserView
				            range: 0-(Amount of Messages in UserView (use "screenBuffer_getFillLevel"))
		return: true  - transfer correctly
			      false -	error occured
*/
bool screenBuffer_getMessageAtPosition(Canmsg & msg, int pos)
{
    if(screenBufferInitialized && (pos >= 0) && (pos < screenBufferUserViewFillLevel))
    {
        msg = *(screenBufferUserView[pos]);
        return true;
    }
    else
    {
        return false;
    }
}

/*
		returns the ammount of messages currently stored in userView
		return: int -	number of messages stored in userView
*/
int screenBuffer_getFillLevel(void)
{
    return screenBufferUserViewFillLevel;
}
