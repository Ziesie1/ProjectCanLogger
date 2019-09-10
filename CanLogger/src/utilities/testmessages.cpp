#include "utilities/testmessages.hpp"
#include <Arduino.h>
#include "display/screenBuffer.hpp"
#include "can/CanUtility.hpp"
#include "utilities/SerialCommunication.hpp"

bool testmessagesSend = false;

/*
    function that puts the CAN hardware in Loopback mode and sets it to different Communication 
    baud rates to emulate recieving messages as soon as the screenBuffer is updatet to test the 
    functionality of different settings of the recieve mode. 
    the function automatically resets the hardware to the settings before the test was startet
    note that the funsction is just a system Test and provides only benefit to test the controller 
    while lacking a physical bus system
*/
void sendTestmessages(void)
{
    if(screenBuffer_updateStatus() && !testmessagesSend)
    {
        CAN_TransmissionMode currentMode = CanUtility_getTransmissionMode();
        if(currentMode == CAN_TransmissionMode_Normal)
        {  
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Loopback);
        }
        else if(currentMode == CAN_TransmissionMode_Silent)
        {
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Silent_Loopback);
        }

        CAN_SpeedTypedef currentSpeed = CanUtility_getTransmissionSpeed();

        Canmsg send{};
        for(int i=0;i<10;i++)
        {
            switch(i)
            {
                case 1: send = Canmsg{0x2, 0, false, false, 1000, 8, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0}; 
                        CanUtility_setTransmissionSpeed(CAN_500_KBIT);
                        break;
                case 2: send = Canmsg{0xff, 34, true, false, 1000, 8, 0x11, 0x33, 0x55, 0x77, 0x99, 0xbb, 0xdd, 0xff};  
                        CanUtility_setTransmissionSpeed(CAN_400_KBIT);
                        break;
                case 3: send = Canmsg{0x45, 0, false, true, 1000, 5, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};  
                        CanUtility_setTransmissionSpeed(CAN_250_KBIT);
                        break;
                case 4: send = Canmsg{0x5, 0, false, true, 1000, 8, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};  
                        CanUtility_setTransmissionSpeed(CAN_200_KBIT);
                        break;
                case 5: send = Canmsg{0x17, 0, false, false, 1000, 3, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};  
                        CanUtility_setTransmissionSpeed(CAN_125_KBIT);
                        break;
                case 6: send = Canmsg{0xc, 1, true, false, 1000, 6, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff};  
                        CanUtility_setTransmissionSpeed(CAN_100_KBIT);
                        break;
                case 7: send = Canmsg{send.maxStdId, send.maxExtId, true, false, 1000, 8, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0}; break;
                case 8: send = Canmsg{0xa, 0, false, false, 1000, 8, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0}; break;
                case 9: send = Canmsg{0xb, 0, false, false, 1000, 8, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0}; break;
            }
            CanUtility_SendMessage(&send);
            delay(10);
        }

        CanUtility_setTransmissionMode(currentMode);
        CanUtility_setTransmissionSpeed(currentSpeed);
        testmessagesSend = true;
    }
    else if(!screenBuffer_updateStatus())
    {
        testmessagesSend = false;
    }
}